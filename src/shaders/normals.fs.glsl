#version 330 core

in vec3 vFragNormal;

out vec3 fFragColor;

void main(){
    vec3 normalizedNormal = normalize(vFragNormal);
    fFragColor = normalizedNormal;
}