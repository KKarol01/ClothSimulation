cmake_minimum_required (VERSION 3.8)
project ("kd")

add_executable (kd 
"src/main.cpp"
"src/glad.c"
"src/engine.hpp" 
"src/engine.cpp" 
"src/hw_ui/window.hpp" 
"src/hw_ui/window.cpp" 
  
"src/shader/shader.hpp" 
"src/shader/shader.cpp" 
"include/stbi_image.h" 
"src/buffer/buffer.hpp" 
"src/buffer/buffer.cpp" 
"src/hw_ui/imgui.hpp" 
"src/hw_ui/imgui.cpp" 
"src/camera/camera.hpp" 
"src/camera/camera.cpp" 
"src/balls/ball.hpp" 
"src/balls/ball.cpp"   
"src/balls/cloth.hpp" 
"src/balls/cloth.cpp" 
"src/hw_ui/controller.hpp" 
"src/hw_ui/controller.cpp" 
"src/update_command/update_command.hpp" 
"src/update_command/update_command.cpp"
)
add_subdirectory("src/3rdparty")
target_include_directories(kd PRIVATE "include/" "src/3rdparty/")
target_link_directories(kd PRIVATE "lib/")
target_link_libraries(kd PRIVATE glfw3dll imgui)
set_property(TARGET kd PROPERTY CXX_STANDARD 20)
set_property(TARGET kd PROPERTY CXX_STANDARD_REQUIRED ON)

add_custom_target(copy_shaders ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/runtime/shaders ${CMAKE_BINARY_DIR}/shaders)
configure_file(${CMAKE_SOURCE_DIR}/src/runtime/glfw3.dll ${CMAKE_BINARY_DIR}/glfw3.dll COPYONLY)