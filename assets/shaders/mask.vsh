#version 330
layout (location = 0) in vec3 aPos;

out vec3 pos;
void main() {
    pos = aPos;
    gl_Position = vec4(aPos, 1);
}