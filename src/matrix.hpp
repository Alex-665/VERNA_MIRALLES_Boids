#pragma once
#include "freeflyCamera.hpp"
#include "arpenteur.hpp"
#include "p6/p6.h"
#include "include_glm.hpp"
#include "boid.hpp"

struct globalMatrix {
    glm::mat4 MVMatrix;
    glm::mat4 ProjMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 ViewMatrix;
};

struct uGlobalMatrix {
    GLint uMVMatrix;
    GLint uMVPMatrix;
    GLint uNormalMatrix;
};

void matricesCube(globalMatrix &gm, uGlobalMatrix &ugm);
void matricesSwan(globalMatrix &gm, uGlobalMatrix &ugm, Arpenteur player);
void matricesBoids(globalMatrix &gm, uGlobalMatrix &ugm, glm::mat4 model_matrix);
void getUniformLocations(const bool instancing, const p6::Shader &shader, uGlobalMatrix &ugm);

glm::mat4 translate(float tx, float ty, float tz);
glm::mat4 scale(float sx, float sy, float sz);

