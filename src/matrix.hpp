#pragma once
#include "p6/p6.h"
#include "include_glm.hpp"
#include "boid.hpp"

struct globalMatrix {
    glm::mat4 MVMatrix;
    glm::mat4 ProjMatrix;
    glm::mat4 NormalMatrix;
};

struct uGlobalMatrix {
    GLint uMVMatrix;
    GLint uMVPMatrix;
    GLint uNormalMatrix;
};

void moveBoid(globalMatrix &gm, uGlobalMatrix &ugm, const Boid &b, const float time);
void getUniformLocations(const p6::Shader &shader, uGlobalMatrix &ugm);

glm::mat4 translate(float tx, float ty, float tz);
glm::mat4 scale(float sx, float sy, float sz);

