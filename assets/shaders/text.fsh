#version 330 core
in vec2 tex;
out vec4 color;

uniform sampler2D baseTex;
uniform vec3 tColor;

void main()
{    
    color = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(baseTex, tex).r);
    color = vec4(tColor, 1.0) * sampled;
}  