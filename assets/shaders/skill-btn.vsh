#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 proj;
out vec2 texCoord;

void main() {
    gl_Position = proj * vec4(pos.xy, 0, 1);
    gl_Position.z = -0.9;
    texCoord = tex;
}