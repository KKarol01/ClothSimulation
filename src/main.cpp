#include <iostream>

#include <OBJ_Loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stbi_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <imgui/imgui.h>

#include "engine.hpp"
#include "shader/shader.hpp"
#include "balls/cloth.hpp"
#include "buffer/buffer.hpp"

#define WW 640
#define WH 480
#define WT "title"

int main() {
    using namespace ph;

    try {
        Engine::Init(WW, WH, WT);
        const auto window = Engine::GetWindow();
        const auto camera = Engine::GetCamera();

        const auto skybox_shader = std::make_unique<Shader>("shaders/", "skybox");

        Cloth simulation{150, 150};

        float time_current, time_past, time_delta{0.f};
        time_current = time_past = static_cast<float>(glfwGetTime());

        float simulation_time = 0.f;
        UI::Draw([&] {
            if (ImGui::Begin("FPS COUNTER")) {
                ImGui::Text("%f", 1.0f / time_delta);
                ImGui::Text("KBANDMOUSE: %i", typeid(*Engine::GetController()) == typeid(KeyboardAndMouse));
                ImGui::Text("Simulation time: %.2fms", simulation_time * 1000.0f);
                if (ImGui::Button("Recompile")) { simulation._RECOMPILE(); }
            }
            ImGui::End();
        });

        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        while (!window->ShouldClose()) {
            time_current = static_cast<float>(glfwGetTime());
            time_delta   = time_current - time_past;
            time_past    = time_current;
            glfwPollEvents();
            ph::Engine::Update();

            if (!window->focused) continue;

            camera->Update();
            {
                if (glfwGetKey(window->GLFWPTR(), GLFW_KEY_ESCAPE) == GLFW_PRESS) window->SetShouldClose(true);
            }

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float prev_result = simulation_time;
            simulation_time   = glfwGetTime();
            simulation.Draw(time_delta);
            simulation_time = glfwGetTime() - simulation_time;
            simulation_time = (prev_result + simulation_time) * 0.5f;

            UI::Render();
            window->Swap();
        }

        Engine::Terminate();
    } catch (const std::exception &e) { std::cout << e.what(); }

    return 0;
};