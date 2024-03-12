#version 460 core
#define PI 3.141592

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 vstc;
out vec3 vsnorm;
void main() {
    vstc        = tc;
    vsnorm      = (model * vec4(normal, 0.0)).xyz;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
