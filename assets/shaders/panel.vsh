#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 aTexCoord;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * vec4(pos, 1.0);
    texCoord = aTexCoord * 10;
}