#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexUV;
//layout(location = 3) in mat4 aInstanceMatrix; //matrix with the translation of our models

out vec3 vFragNormal;
out vec3 vFragPosition;
out vec2 vFragUV;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

void main() {
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    vFragPosition = vec3 (uMVMatrix * vertexPosition);
    vFragNormal = vec3 (uNormalMatrix * vertexNormal);
    vFragUV = aVertexUV;
    gl_Position = vec4(uMVPMatrix * vertexPosition);
};
