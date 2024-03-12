#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string_view>
#include <glm/glm.hpp>

#include "hw_ui/window.hpp"
#include "hw_ui/controller.hpp"
#include "camera/camera.hpp"
#include "hw_ui/imgui.hpp"
#include "update_command/update_command.hpp"

namespace ph {
    typedef void (*engine_funcptr)(float);

    class Engine {

        inline static std::unique_ptr<ph::Window> _window;
        inline static std::unique_ptr<ph::Camera> _camera;
        inline static std::unique_ptr<ph::Controller> _controller;

        inline static std::map<size_t, std::unique_ptr<ph::UpdateCommand>> _on_update_callbacks{};

        friend void joystick_connect(int, int);

      public:
        static void Init(uint32_t window_w, uint32_t window_h, std::string_view window_title);
        static void Terminate();
        static void Update();

        static ph::Window *GetWindow();
        static ph::Camera *GetCamera();
        static ph::Controller *GetController();

        static glm::mat4 projection;

        static size_t OnUpdateSubscribe(const ph::UpdateCommand &command);
        static void OnUpdateUnSubscribe(size_t);
    };
} // namespace ph