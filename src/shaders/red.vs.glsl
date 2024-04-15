layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexUV;
#ifdef INSTANCING
    layout(location = 3) in mat4 aInstanceMatrix; //matrix with the translation of our models
#endif

out vec3 vFragNormal;
out vec3 vFragPosition;
out vec2 vFragUV;

uniform mat4 uMVPMatrix;
#ifndef INSTANCING
    uniform mat4 uMVMatrix;
#endif
uniform mat4 uNormalMatrix;

void main() {
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    #ifdef INSTANCING
        vFragPosition = vec3 (aInstanceMatrix * vertexPosition);
        vFragNormal = vec3 (uNormalMatrix * vertexNormal);
        gl_Position = vec4(uMVPMatrix * aInstanceMatrix * vertexPosition);
        vFragUV = aVertexUV;
    #else
        vFragPosition = vec3 (uMVMatrix * vertexPosition);
        vFragNormal = vec3 (uMVMatrix * vertexNormal);
        vFragUV = aVertexUV;
        gl_Position = vec4(uMVPMatrix * vertexPosition);
    #endif
};
