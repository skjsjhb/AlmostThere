#version 330

uniform float args[8];
uniform sampler2D baseTex;

in vec2 texCoord;

out vec4 color;

void main() {
    color = texture(baseTex, texCoord);
}