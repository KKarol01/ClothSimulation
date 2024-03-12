#include "cloth.hpp"
#include "../engine.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>

#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <stbi_image.h>

Cloth::Cloth(size_t rows, size_t cols) : rows(rows), cols(cols) {
    shader          = std::make_unique<Shader>("shaders/", "point");
    compute_shader  = std::make_unique<Shader>("shaders/", "cloth");
    compute_physics = std::make_unique<Shader>("shaders/", "cloth_physics");

    struct alignas(16) VERTEX {
        alignas(16) glm::vec4 pos_pinned;
        alignas(16) glm::vec4 force_mass;
        alignas(16) glm::vec3 prev_pos;
        alignas(16) glm::vec3 normal;
        alignas(8) glm::vec2 tc;

        VERTEX(glm::vec4 p, glm::vec3 n, glm::vec2 tc) : pos_pinned{p}, normal{n}, tc{tc} {
            prev_pos   = glm::vec3{pos_pinned};
            force_mass = glm::vec4{0.f, 0.f, 0.f, 0.6f};
        }
    };
    struct alignas(16) VERTEX_SHADER {
        glm::u32vec4 indices[6];
        uint32_t indice_count{0};
    };
    std::map<unsigned, VERTEX_SHADER> vdata;
    std::vector<VERTEX> vertices;
    std::vector<unsigned> indices;

    row_dist                  = (8.0f) / static_cast<float>(rows - 1);
    col_dist                  = (8.0f) / static_cast<float>(cols - 1);
    const float ball_row_dist = row_dist;
    const float ball_col_dist = col_dist;

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            glm::vec4 position{-4.f + ball_col_dist * static_cast<float>(col),
                               -4.f + ball_row_dist * static_cast<float>(row),
                               0.f,
                               row > rows - 2};
            glm::vec3 normal{0.f, 0.f, 0.f};
            glm::vec2 texture_coordinates = glm::vec2{position} * 0.5f + 0.5f;

            vertices.emplace_back(position, normal, texture_coordinates);
        }
    }
    for (size_t row = 0; row < rows - 1; ++row) {
        for (size_t col = 0; col < cols - 1; ++col) {
            size_t idx = row * cols + col;
            indices.push_back(idx);
            indices.push_back(idx + 1);
            indices.push_back(idx + cols);

            indices.push_back(idx + 1);
            indices.push_back(idx + cols + 1);
            indices.push_back(idx + cols);
        }
    }
    indice_count = indices.size();
    for (unsigned i = 0; i < indices.size(); i += 3) {
        unsigned i1 = indices[i + 0];
        unsigned i2 = indices[i + 1];
        unsigned i3 = indices[i + 2];

        const auto v1 = glm::vec3{vertices[i1].pos_pinned};
        const auto v2 = glm::vec3{vertices[i2].pos_pinned};
        const auto v3 = glm::vec3{vertices[i3].pos_pinned};

        const bool swap_for_i2 = glm::cross(v2 - v1, v3 - v1) == -glm::cross(v1 - v2, v3 - v2);
        const bool swap_for_i3 = glm::cross(v2 - v1, v3 - v1) == -glm::cross(v1 - v3, v2 - v3);

        // clang-format off
        vdata[i1].indices[vdata[i1].indice_count++] = {i1, i2, i3, 0};
        vdata[i2].indices[vdata[i2].indice_count++] = swap_for_i2 ? glm::u32vec4{i2, i3, i1, 0} : glm::u32vec4{i3, i2, i1, 0};
        vdata[i3].indices[vdata[i3].indice_count++] = swap_for_i3 ? glm::u32vec4{i3, i2, i1, 0} : glm::u32vec4{i3, i1, i2, 0};
        // clang-format on
    }

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);
    glCreateBuffers(1, &vbosmooth);

    glNamedBufferStorage(vbo, vertices.size() * sizeof(vertices[0]), vertices.data(), 0);
    glNamedBufferStorage(ebo, indices.size() * sizeof(unsigned), indices.data(), 0);

    std::vector<VERTEX_SHADER> _vdata;
    for (const auto &[k, v] : vdata) _vdata.push_back(v);
    glNamedBufferStorage(vbosmooth, vdata.size() * sizeof(vdata[0]), _vdata.data(), 0);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(vertices[0]));
    glVertexArrayElementBuffer(vao, ebo);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 48);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, 64);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);

    int x, y, ch;
    auto data = stbi_load("dywan.jpg", &x, &y, &ch, 0);
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glTextureStorage2D(tex, 8, GL_RGBA8, x, y);
    glTextureSubImage2D(tex, 0, 0, 0, x, y, ch == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glGenerateTextureMipmap(tex);
    stbi_image_free(data);

    vbo_smooth_size = vdata.size() * sizeof(vdata[0]);
    ebo_size        = indices.size() * 4;
    vbo_size        = vertices.size() * sizeof(vertices[0]);

    ph::UI::Draw([&]() {
        ImGui::Begin("Cloth Data");
        ImGui::Text("Vertex buffer size: %ukB", vbo_size / 1024);
        ImGui::Text("Index buffer size: %ukB", ebo_size / 1024);
        ImGui::Text("Smooth Normals buffer size: %ukB", vbo_smooth_size / 1024);
        ImGui::End();
    });
}

void Cloth::Draw(float delta_time) {
    // Update(delta_time);

    compute_physics->Use();
    compute_physics->SetUint("cols", cols);
    compute_physics->SetUint("rows", rows);
    compute_physics->SetFloat("time", glfwGetTime());
    compute_physics->SetFloat("delta_time", delta_time);
    compute_physics->SetFloat("row_dist", row_dist);
    compute_physics->SetFloat("col_dist", col_dist);
    compute_physics->SetFloat3("gravity_force", gravity_force);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ebo);

    compute_physics->SetUint("offset", 0u);
    glDispatchCompute(glm::floor(cols / 2), 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    compute_physics->SetUint("offset", 1u);
    glDispatchCompute(glm::floor((cols - 1) / 2), 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    compute_shader->Use();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ebo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vbosmooth);
    glDispatchCompute(cols, rows, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glBindTextureUnit(0, tex);
    glBindVertexArray(vao);
    shader->Use();
    shader->SetMat4("model", glm::mat4{1.f});
    shader->SetMat4("view", ph::Engine::GetCamera()->ViewMatrix());
    shader->SetMat4("projection", ph::Engine::projection);
    glDrawElements(GL_TRIANGLES, indice_count, GL_UNSIGNED_INT, 0);
}

void Cloth::_RECOMPILE() {
    shader->Recompile();
    compute_shader->Recompile();
    compute_physics->Recompile();
}
