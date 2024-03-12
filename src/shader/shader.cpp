#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static std::string ReadFile(std::string_view path) {
    std::ifstream file{path.data()};
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
static void LoadCompileShader(std::string_view path, uint32_t program, uint32_t type) {
    const auto shader_src = ReadFile(path.data());
    const char *_src      = shader_src.c_str();

    const auto shader_handle = glCreateShader(type);
    glShaderSource(shader_handle, 1, &_src, 0);
    glCompileShader(shader_handle);

    int success;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        int log_size;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_size);
        char *log = new char[log_size];

        glGetShaderInfoLog(shader_handle, log_size, 0, log);

        std::string comp_msg{"Shader compilation failure at:\npath: \""};
        comp_msg.append(path).append("\"\nFor reason: \"").append(log).append("\"");

        delete[] log; // at this point, the message is already appended to the string above.
        throw std::runtime_error{comp_msg.c_str()};
    }

    glAttachShader(program, shader_handle);
    glDeleteShader(shader_handle);
}

Shader::Shader(std::string_view path, std::string_view base_name) : path{path}, base_name{base_name} {
    if (!path.ends_with("/"))
        throw std::runtime_error{"Shader::Shader(view, view): Path to a directory should always end with \"/\""};

    if (!std::filesystem::exists(path)) {
        std::string msg{"Shader::Shader(view,view): Supplied directory \""};
        msg.append(path);
        msg.append("\" does not exists");

        throw std::runtime_error{msg.c_str()};
    }

    bool vs = false, fs = false, gs = false, cs = false;
    for (const auto &i : std::filesystem::directory_iterator{path}) {
        const auto fn = i.path().filename().string();
        if (!fn.starts_with(base_name)) continue;

        const auto ext = fn.substr(fn.find_last_of(".") + 1);
        vs             = vs || ext == "vert";
        fs             = fs || ext == "frag";
        gs             = gs || ext == "geom";
        cs             = cs || ext == "comp";
    }

    if ((!vs || !fs) && !cs)
        throw std::runtime_error{
            "Shader::Shader(view,view): Not enough shader source files: .vs and .fs are required or only .cs."};

    uint32_t shader_handle;
    std::string path_noext{path};
    path_noext.append(base_name);
    std::string shader_src;

    program = glCreateProgram();
    if (vs) LoadCompileShader(path_noext + ".vert", program, GL_VERTEX_SHADER);
    if (fs) LoadCompileShader(path_noext + ".frag", program, GL_FRAGMENT_SHADER);
    if (gs) LoadCompileShader(path_noext + ".geom", program, GL_GEOMETRY_SHADER);
    if (cs) LoadCompileShader(path_noext + ".comp", program, GL_COMPUTE_SHADER);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int log_size;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        char *log = new char[log_size];
        glGetProgramInfoLog(program, log_size, 0, log);
        std::string msg{"Shader program linking error at:\npath: \""};
        msg.append(path_noext).append("\"\nfor reason: \"").append(log).append("\"");

        delete[] log;
        throw std::runtime_error{msg.c_str()};
    }
}

Shader::~Shader() { glDeleteProgram(program); }

void Shader::Use() const { glUseProgram(program); }

void Shader::Recompile() {
    try {
        Shader sh{path, base_name};
        glUseProgram(0);
        glDeleteProgram(program);
        program    = sh.program;
        sh.program = 0;
    } catch (const std::exception &e) {
        std::cerr << "Shader::Recompile(): Could not recompile shader for this reason: " << e.what();
    }
}

void Shader::SetFloat(std::string_view n, float m) const {
    glUniform1fv(glGetUniformLocation(program, n.data()), 1, &m);
}

void Shader::SetFloat2(std::string_view n, const glm::vec2 &m) const {
    glUniform2fv(glGetUniformLocation(program, n.data()), 1, glm::value_ptr(m));
}

void Shader::SetInt(std::string_view n, int m) const { glUniform1i(glGetUniformLocation(program, n.data()), m); }

void Shader::SetUint(std::string_view n, unsigned m) const { glUniform1ui(glGetUniformLocation(program, n.data()), m); }

void Shader::SetFloat3(std::string_view n, const glm::vec3 &m) const {
    glUniform3fv(glGetUniformLocation(program, n.data()), 1, glm::value_ptr(m));
}

void Shader::SetMat4(std::string_view n, const glm::mat4 &m) const {
    glUniformMatrix4fv(glGetUniformLocation(program, n.data()), 1, GL_FALSE, glm::value_ptr(m));
}
