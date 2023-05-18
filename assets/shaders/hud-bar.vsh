#version 330
layout (location = 0) in vec3 pos;

uniform mat4 proj;

void main() {
    gl_Position = proj * vec4(pos.xy, 0, 1);
    gl_Position.z = -0.9;
}