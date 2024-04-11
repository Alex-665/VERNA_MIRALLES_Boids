#version 330 core

in vec2 vFragUV;
uniform sampler2D uTexture;

out vec4 fFragColor;

void main() {
    vec4 tmp = texture(uTexture, vFragUV);
    if (tmp.a < 0.1) discard;
    fFragColor = tmp;
};

