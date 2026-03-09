#version 410 core

layout (location = 0) in vec2 fragUV;

out vec4 FragColor;

uniform sampler2D fontAtlas;
uniform vec4 textColor;

void main() {
    float coverage = texture(fontAtlas, fragUV).r;
    FragColor = vec4(textColor.rgb, textColor.a * coverage);
}
