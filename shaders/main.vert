#version 410 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float vertColorIndex;

layout (location = 0) out vec4 color;

uniform vec2 screenSize;


vec4 COLORS[9] = vec4[9](
    vec4(0.191f, 0.777f, 0.933f, 1.0f), // I
    vec4(0.352f, 0.395f, 0.676f, 1.0f), // J
    vec4(0.934f, 0.473f, 0.129f, 1.0f), // L
    vec4(0.965f, 0.824f, 0.031f, 1.0f), // O
    vec4(0.258f, 0.711f, 0.258f, 1.0f), // S
    vec4(0.676f, 0.301f, 0.609f, 1.0f), // T
    vec4(0.934f, 0.125f, 0.160f, 1.0f), // Z
    vec4(0.000f, 0.000f, 0.000f, 1.0f), // EMPTY
    vec4(1.000f, 1.000f, 1.000f, 1.0f)  // GHOST
);

void main() {
    vec2 ndc = (pos / screenSize) * 2.0 - 1.0;
    ndc.y = -ndc.y;
    gl_Position = vec4(ndc, 0.0, 1.0);
    

    if (int(vertColorIndex) < 9) {
        color = COLORS[int(vertColorIndex)];
    } else {
        // Fallback
        color = vec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta for debugging
    }
}
