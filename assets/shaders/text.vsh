#version 330
layout (location = 0) in vec4 vert;
out vec2 tex;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(vert.xy, 0, 1);
    gl_Position.z = 0.9;
    tex = vert.zw;
}