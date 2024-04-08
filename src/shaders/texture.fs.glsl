#version 330 core

in vec2 vFragUV;
uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
    fFragColor = texture(uTexture, vFragUV).rgb;
};

