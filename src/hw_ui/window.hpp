#pragma once

#include <string>
#include <string_view>
#include <utility>

struct GLFWwindow;

namespace ph {
    class Window {
        GLFWwindow *_window = nullptr;

      public:
        std::string title{""};
        uint32_t width{0}, height{0};
        bool valid{false}, focused{true};
        Window() = default;
        Window(uint32_t window_w, uint32_t window_h, std::string_view window_title);
        ~Window();

        bool ShouldClose() const;
        void SetShouldClose(int);
        std::pair<float, float> GetCursorPos() const;
        GLFWwindow *GLFWPTR() const;
        float AspectRatio() const;

        void Resize(int w, int h);
        void MakeCurrent() const;
        void Swap() const;
    };
} // namespace ph