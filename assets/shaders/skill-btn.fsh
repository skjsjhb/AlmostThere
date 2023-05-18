#version 330

uniform float args[8];
uniform sampler2D baseTex;

in vec2 texCoord;

out vec4 color;

void main() {
    float stat = args[0];
    float blinkRate = args[1];
    color = texture(baseTex, texCoord);
    if (stat < 1 && stat >= 0) {
        float cx = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
        color = vec4(cx, cx, cx, 0.4);
    } else if (stat == 2) {
        color.a = 1.0;
    } else {
        color.a *= 0.6 + 0.2 * sin(blinkRate * 8);
    }
}