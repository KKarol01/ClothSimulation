cmake_minimum_required (VERSION 3.8)

add_library(imgui 
"imgui/imgui.cpp"
"imgui/imgui_draw.cpp"
"imgui/imgui_impl_glfw.cpp"
"imgui/imgui_impl_opengl3.cpp"
"imgui/imgui_tables.cpp"
"imgui/imgui_widgets.cpp"
)
target_include_directories(imgui PRIVATE "${CMAKE_SOURCE_DIR}/include/")