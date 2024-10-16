#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float vertColorIndex;

layout (location = 0) out vec4 color;


vec4 COLORS[7] = vec4[7](
    vec4(0.191f, 0.777f, 0.933f, 1.0f), // I
    vec4(0.352f, 0.395f, 0.676f, 1.0f), // J
    vec4(0.934f, 0.473f, 0.129f, 1.0f), // L
    vec4(0.965f, 0.824f, 0.031f, 1.0f), // O
    vec4(0.258f, 0.711f, 0.258f, 1.0f), // S
    vec4(0.676f, 0.301f, 0.609f, 1.0f), // T
    vec4(0.934f, 0.125f, 0.160f, 1.0f)  // Z
);

void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    

    if (int(vertColorIndex) < 7) {
        color = COLORS[int(vertColorIndex)];
    } else {
        color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
