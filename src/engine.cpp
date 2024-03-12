#include <glad/glad.h>
#include "engine.hpp"
#include "update_command/update_command.hpp"

#include <iostream>
#include <cassert>
#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

static void window_resize(GLFWwindow *window, int w, int h);
namespace ph {
    static void joystick_connect(int jid, int event);
}

void ph::Engine::Init(uint32_t window_w, uint32_t window_h, std::string_view window_title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_FORWARD_COMPAT);

    _window = std::make_unique<ph::Window>(window_w, window_h, window_title);
    if (!_window->valid) throw std::runtime_error{"Created window is not valid."};

    _window->MakeCurrent();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error{"Loading GL Loader failed."};

    glViewport(0, 0, window_w, window_h);
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glfwSetFramebufferSizeCallback(_window->GLFWPTR(), (GLFWframebuffersizefun)window_resize);
    glfwSetJoystickCallback((GLFWjoystickfun)ph::joystick_connect);
    glfwSwapInterval(1);

    _camera = std::make_unique<Camera>();

    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        _controller = std::make_unique<Gamepad>();
    } else {
        _controller = std::make_unique<KeyboardAndMouse>();
    }

    projection = glm::perspective(glm::radians(90.0f), _window->AspectRatio(), 0.01f, 100.0f);
    UI::Init();
}

ph::Window *ph::Engine::GetWindow() { return _window.get(); }
ph::Camera *ph::Engine::GetCamera() { return _camera.get(); }
ph::Controller *ph::Engine::GetController() { return _controller.get(); }

void ph::Engine::Terminate() {
    _controller.get_deleter()(_controller.release()); //
    _on_update_callbacks.clear();                     // Without, the app doesn't quit properly.

    UI::Terminate();
    glfwTerminate();
}

void ph::Engine::Update() {
    for (auto &[_, v] : _on_update_callbacks) { v->operator()(0.f); }
}

glm::mat4 ph::Engine::projection = glm::mat4{};

static size_t cb_id = 0ULL;
size_t ph::Engine::OnUpdateSubscribe(const ph::UpdateCommand &command) {
    _on_update_callbacks[cb_id].reset(command.clone());
    return cb_id++;
}

void ph::Engine::OnUpdateUnSubscribe(size_t id) { _on_update_callbacks.erase(id); }

static void window_resize(GLFWwindow *window, int w, int h) {
    ph::Engine::GetWindow()->Resize(w, h);
    ph::Engine::projection
        = glm::perspective(glm::radians(90.0f), ph::Engine::GetWindow()->AspectRatio(), 0.01f, 100.0f);
    auto wind = ph::Engine::GetWindow();
}
static void ph::joystick_connect(int jid, int event) {
    if (event == GLFW_CONNECTED) {
        ph::Engine::_controller = std::make_unique<ph::Gamepad>();
    } else if (event == GLFW_DISCONNECTED) {
        ph::Engine::_controller = std::make_unique<ph::KeyboardAndMouse>();
    }
}