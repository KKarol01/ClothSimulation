#version 460 core

layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 color[];
out vec3 _c;

vec3 cin(int a, int b, int c) {
    return cross(color[b] - color[a], color[c] - color[a]);
    return cross(gl_in[b].gl_Position.xyz - gl_in[a].gl_Position.xyz,
                 gl_in[c].gl_Position.xyz - gl_in[a].gl_Position.xyz);
}

void main() {
    vec3 base   = 1. * normalize(cin(0, 2, 4));
    vec3 left   = 1. * normalize(cin(4, 5, 0));
    vec3 right  = 1. * normalize(cin(2, 3, 4));
    vec3 bottom = 1. * normalize(cin(0, 1, 2));

    gl_Position = gl_in[0].gl_Position;
    _c          = base + left; //+ bottom;
    _c          = normalize(_c);
    EmitVertex();

    _c          = base + bottom + right;
    _c          = normalize(_c);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    _c          = base + left + right;
    _c          = normalize(_c);
    gl_Position = gl_in[4].gl_Position;
    EmitVertex();

    EndPrimitive();
}
