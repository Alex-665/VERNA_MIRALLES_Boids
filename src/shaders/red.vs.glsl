#version 330 core

layout(location = 0) in vec2 aVertexPosition;

uniform mat3 u_translate_boid_matrix;

void main() {
    vec2 vVertexPosition = vec2(u_translate_boid_matrix * vec3(aVertexPosition, 1)).xy;
    gl_Position = vec4(vVertexPosition, 0, 1);
};
