#version 330
in vec2 texCoord;
out vec4 color;

uniform sampler2D baseTex;
uniform float args[8];

void main () {
    color = texture(baseTex, texCoord);
    if(color.a == 0){
        discard;
    }
    color.a *= args[0];
}