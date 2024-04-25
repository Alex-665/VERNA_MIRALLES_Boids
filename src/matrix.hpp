#pragma once
#include "freeflyCamera.hpp"
#include "p6/p6.h"
#include "include_glm.hpp"
#include "boid.hpp"

struct GlobalMatrix {
    glm::mat4 MVMatrix;
    glm::mat4 proj_matrix;
    glm::mat4 NormalMatrix;
    glm::mat4 view_matrix;
};

struct uGlobalMatrix {
    GLint uMVMatrix;
    GLint uMVPMatrix;
    GLint uNormalMatrix;
};

void matrices_cube(GlobalMatrix &gm, uGlobalMatrix &ugm);
void matricesSwan(GlobalMatrix &gm, uGlobalMatrix &ugm, Arpenteur player);
void matrices_boids(GlobalMatrix &gm, uGlobalMatrix &ugm, glm::mat4 model_matrix);
void get_uniform_locations(const bool instancing, const p6::Shader &shader, uGlobalMatrix &ugm);

glm::mat4 translate(float tx, float ty, float tz);
glm::mat4 scale(float sx, float sy, float sz);

