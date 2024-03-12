#include "window.hpp"
#include "../engine.hpp"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

ph::Window::Window(uint32_t window_w, uint32_t window_h, std::string_view window_title)
    : title(window_title.data()), width(window_w), height(window_h),
      _window(
          glfwCreateWindow(static_cast<int>(window_w), static_cast<int>(window_h), window_title.data(), NULL, NULL)) {
    if (_window) valid = true;

    glfwSetWindowFocusCallback(_window,
                               [](GLFWwindow *window, int focus) { ph::Engine::GetWindow()->focused = focus; });
}

ph::Window::~Window() { glfwDestroyWindow(_window); }

void ph::Window::Resize(int w, int h) {
    width  = w;
    height = h;
    glViewport(0, 0, w, h);
}

void ph::Window::MakeCurrent() const { glfwMakeContextCurrent(_window); }

bool ph::Window::ShouldClose() const { return glfwWindowShouldClose(_window); }

void ph::Window::SetShouldClose(int v) { glfwSetWindowShouldClose(_window, v); }

std::pair<float, float> ph::Window::GetCursorPos() const {
    double x, y;
    glfwGetCursorPos(_window, &x, &y);
    x = glm::max(0.0, glm::min(static_cast<double>(width), x));
    y = glm::max(0.0, glm::min(static_cast<double>(height), y));
    return std::pair(static_cast<float>(x), static_cast<float>(y));
}

GLFWwindow *ph::Window::GLFWPTR() const { return _window; }

float ph::Window::AspectRatio() const { return static_cast<float>(width) / static_cast<float>(height); }

void ph::Window::Swap() const { glfwSwapBuffers(_window); }
