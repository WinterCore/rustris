#version 410 core

layout (location = 0) in vec3 pos;

out vec4 color;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);

    if (gl_VertexID <= 3) {
        color = vec4(0.5f, 0.3f, 0.0f, 1.0f);
    } else {
        color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
}
