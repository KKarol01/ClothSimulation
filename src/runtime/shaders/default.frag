#version 460 core

in vec3 vsnorm;

out vec4 FRAG_COLOR;
void main() {
	FRAG_COLOR = vec4(vsnorm, 1.0);
}