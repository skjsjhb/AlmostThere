#version 330

uniform float args[8];
out vec4 ocol;

void main() {
    ocol = vec4(args[0], args[1], args[2], 1);
}