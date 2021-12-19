#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoor;
uniform mat4 transform;

out vec2 texCoord;

void main() {
    gl_Position = transform * vec4(position, 1.0);
    texCoord = texCoor;
}