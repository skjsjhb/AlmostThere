#version 330
layout (location = 0) in vec3 pos;

void main() {
    gl_Position = vec4(pos.xy, 0.9, 1);
}