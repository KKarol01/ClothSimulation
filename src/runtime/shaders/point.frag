#version 460 core

in vec2 vstc;
in vec3 vsnorm;

layout(binding = 0) uniform sampler2D tex;
out vec4 FRAG_COLOR;
void main() {
    vec3 col = texture(tex, vstc).rgb;
    FRAG_COLOR =vec4(col * vsnorm, 1.0f - length(col));
}