#version 330

in vec2 texCoord;
out vec4 color;
uniform sampler2D baseTex;
uniform float time;

void main() {
    color = texture(baseTex, texCoord) - vec4(0, 0, 0, 0.4) * (sin(time) + 1);
}