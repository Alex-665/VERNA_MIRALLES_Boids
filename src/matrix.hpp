#pragma once
#include "freeflyCamera.hpp"
#include "p6/p6.h"
#include "include_glm.hpp"
#include "boid.hpp"
#include "arpenteur.hpp"

struct GlobalMatrix {
    glm::mat4 m_v_matrix;
    glm::mat4 proj_matrix;
    glm::mat4 normal_matrix;
    glm::mat4 view_matrix;
};

struct uGlobalMatrix {
    GLint u_m_v_matrix;
    GLint u_m_v_p_matrix;
    GLint u_normal_matrix;
};

void matrices_cube(GlobalMatrix &gm, uGlobalMatrix &ugm);
void matrices_swan(GlobalMatrix &gm, uGlobalMatrix &ugm, Arpenteur player);
void matrices_boids(GlobalMatrix &gm, uGlobalMatrix &ugm, glm::mat4 model_matrix);
void get_uniform_locations(const bool instancing, const p6::Shader &shader, uGlobalMatrix &ugm);

glm::mat4 translate(float tx, float ty, float tz);
glm::mat4 scale(float sx, float sy, float sz);
