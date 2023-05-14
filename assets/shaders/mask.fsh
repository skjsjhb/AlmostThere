#version 330

in vec3 pos;

uniform float args[8];

out vec4 outColor;

void main() {
    float beginRad = args[4];
    vec4 color = vec4(args[0], args[1], args[2], args[3]);
    float d = distance(pos.xyz, vec3(0, 0, 0));
    if (d <= beginRad) {
        discard;
    } else {
       float pct = (d - beginRad) / distance(vec3(1, 1, 0), vec3(0, 0, 0)) - 0.1;
       outColor = color;
       outColor.a *= pct;
    }
}