#version 330 core

in vec2 vFragUV;
in vec3 vFragPosition;
in vec3 vFragNormal;

uniform sampler2D uTexture;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec4 uLightPos[2];
uniform vec3 uLightIntensity[2];

out vec4 fFragColor;

//vec3 blinnPhong() {
//    vec3 w0 = normalize(-vFragPosition);
//    vec3 wi = normalize(uLightPos_vs - vFragPosition);
//    vec3 halfVector = (w0 + wi)/2;
//    float d = distance(uLightPos_vs, vFragPosition);
//    return (uLightIntensity/(d*d)) * (uKd * dot(wi, vFragNormal) + uKs * pow(dot(halfVector, vFragNormal), uShininess));
//}

vec3 blinnPhongMulti(int n) {
    vec3 lightFactor = vec3(0,0,0);
    vec3 w0 = normalize(-vFragPosition);
    for(int i = 0 ; i < n ; i++)
    {
        vec3 wi = normalize(vec3(uLightPos[i]) - vFragPosition);
        vec3 halfVector = (w0 + wi)/2;
        float d = distance(vec3(uLightPos[i]), vFragPosition);
        if(d<1) lightFactor += (uLightIntensity[i]) * (uKd * max(dot(wi, vFragNormal), 0.0) + uKs * pow(max(dot(halfVector, vFragNormal), 0.0), uShininess));
        else lightFactor += (uLightIntensity[i]/(d*d)) * (uKd * max(dot(wi, vFragNormal), 0.0) + uKs * pow(max(dot(halfVector, vFragNormal), 0.0), uShininess));
    }
    return clamp(lightFactor, 0.3, 1);
}

void main() {
    vec4 tmp = texture(uTexture, vFragUV);
    if (tmp.a < 0.1) discard;
    vec4 lightFactor = vec4(blinnPhongMulti(2),1);
    fFragColor = lightFactor * tmp;
};