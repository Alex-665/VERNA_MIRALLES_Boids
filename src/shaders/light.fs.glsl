#version 330 core

in vec2 vFragUV;
in vec3 vFragPosition;
in vec3 vFragNormal;

uniform sampler2D uTexture;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

out vec4 fFragColor;

vec3 blinnPhong() {
    vec3 w0 = normalize(-vFragPosition);
    vec3 wi = normalize(uLightDir_vs);
    vec3 halfVector = (w0 + wi)/2;
    return uLightIntensity * (uKd * dot(wi, vFragNormal) + uKs * pow(dot(halfVector, vFragNormal), uShininess));
}

void main() {
    vec4 tmp = texture(uTexture, vFragUV);
    if (tmp.a < 0.1) discard;
    vec4 lightFactor = vec4(blinnPhong(),1);
    fFragColor = lightFactor * tmp;
};

