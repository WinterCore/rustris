#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float vertColorIndex;

layout (location = 0) out vec4 color;


vec4 COLORS[2] = vec4[2](
    vec4(0.0f, 0.0f, 0.0f, 1.0f),
    vec4(1.0f, 0.0f, 0.0f, 1.0f)
);

void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    

    color = COLORS[int(vertColorIndex)];
}
