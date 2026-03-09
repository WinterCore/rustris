#version 410 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec2 fragUV;

uniform vec2 screenSize;

void main() {
    vec2 ndc = (pos / screenSize) * 2.0 - 1.0;
    ndc.y = -ndc.y;
    gl_Position = vec4(ndc, 0.0, 1.0);

    fragUV = uv;
}
