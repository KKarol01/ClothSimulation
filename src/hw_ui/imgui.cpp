#include "imgui.hpp"
#include "../engine.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void ph::UI::Init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(ph::Engine::GetWindow()->GLFWPTR(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ph::UI::Draw(std::function<void()> imgui_draw) { draw_callbacks.push_back(imgui_draw); }

void ph::UI::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto &func : draw_callbacks) { func(); }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiIO *ph::UI::GetIO() { return io; }

void ph::UI::Terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
