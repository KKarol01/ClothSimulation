#pragma once

#include <cstdint>
#include <string_view>
#include <glm/glm.hpp>

struct Shader {
    uint32_t program{0};
    std::string path, base_name; 

    Shader() = default;
    Shader(std::string_view path, std::string_view base_name);
    ~Shader();

    void Use() const;
    void Recompile();

    void SetFloat(std::string_view, float) const;
    void SetFloat2(std::string_view, const glm::vec2& m) const;
    void SetInt(std::string_view, int) const;
    void SetUint(std::string_view, unsigned) const;
    void SetFloat3(std::string_view, const glm::vec3 &) const;
    void SetMat4(std::string_view, const glm::mat4 &) const;
};