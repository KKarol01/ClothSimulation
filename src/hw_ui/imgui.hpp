#pragma once
#include <functional>
#include <vector>

struct ImGuiIO;
namespace ph {

    class UI {
        static inline ImGuiIO *io = nullptr;
        static inline std::vector<std::function<void()>> draw_callbacks;

      public:
        static void Init();
        static void Draw(std::function<void()> imgui_draw);
        static void Render();

        static ImGuiIO *GetIO();

        static void Terminate();
    };
} // namespace ph