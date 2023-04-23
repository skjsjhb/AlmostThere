#version 330

in vec3 pos;

uniform vec4 color;
uniform float beginRad;

out vec4 outColor;

void main() {
    float d = distance(pos.xyz, vec3(0, 0, 0));
    if (d <= beginRad) {
        discard;
    } else {
       float pct = (d - beginRad) / distance(vec3(1, 1, 0), vec3(0, 0, 0)) - 0.1;
       outColor = color;
       outColor.a *= pct;
    }
}