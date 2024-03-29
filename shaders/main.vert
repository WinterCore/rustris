#version 410 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float vertColor;

out vec4 color;

void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
