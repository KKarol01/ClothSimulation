#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vsnorm;

void main() {
    vsnorm      = norm;
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
