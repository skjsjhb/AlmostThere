#version 330
in vec2 texCoord;
out vec4 color;

uniform sampler2D baseTex;
uniform vec3 aNormal;
uniform vec3 camDir;

void main () {
    color = texture(baseTex, texCoord);
    if(color.a == 0){
        discard;
    }
    color.a -= 0.3 * abs(dot(normalize(aNormal), normalize(camDir)));
}