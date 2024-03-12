#version 460 core

in vec3 vsnorm;
layout(binding = 0) uniform samplerCube skybox_sampler;
out vec4 FRAG_COLOR;

void main() { FRAG_COLOR = texture(skybox_sampler, vsnorm); }