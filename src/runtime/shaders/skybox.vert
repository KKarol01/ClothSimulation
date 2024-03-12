#version 460 core

layout(location = 0) in vec3 tc;

uniform mat4 view;
uniform mat4 projection;

out vec3 vsnorm;

void main() {
    gl_Position = projection * mat4(mat3(view)) * vec4(tc.xyz, 1.0);
    gl_Position = gl_Position.xyww;
}
