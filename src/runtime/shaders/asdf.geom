#version 460 core

layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 vstc[];
in vec3 color[];
out vec3 _c;
out vec2 vstc_;

vec3 cin(int a, int b, int c) {
	return cross(color[b] - color[a], color[c] - color[a]);
	return cross(gl_in[b].gl_Position.xyz - gl_in[a].gl_Position.xyz, gl_in[c].gl_Position.xyz - gl_in[a].gl_Position.xyz);
}

void main() {
	vec3 base	=	(cin(0, 2, 4));
	vec3 left	=0.*	(-(cin(2, 3, 4)));
	vec3 right	=0.*	((cin(5, 4, 2)));
	vec3 bottom	=0.*	((cin(2, 1, 0)));

	
	gl_Position = gl_in[0].gl_Position;
	_c = normalize(base + left + bottom);
	vstc_ = vstc[0];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	_c = normalize(base + bottom + right);
	vstc_ = vstc[2];
	EmitVertex();

	_c = normalize(base + left + right);
	vstc_ = vstc[4];
	gl_Position = gl_in[4].gl_Position;
	EmitVertex();

	EndPrimitive();
}
