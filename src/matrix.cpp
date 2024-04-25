#include "matrix.hpp"
#include "freeflyCamera.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"

void matrices_cube(GlobalMatrix &gm, uGlobalMatrix &ugm)
{
    gm.m_v_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
    gm.m_v_matrix = glm::scale(gm.m_v_matrix, glm::vec3(15,15,15));
    gm.normal_matrix = glm::transpose(glm::inverse(gm.m_v_matrix));
    glUniformMatrix4fv(ugm.u_m_v_matrix, 1, GL_FALSE, glm::value_ptr(gm.view_matrix * gm.m_v_matrix));
    glUniformMatrix4fv(ugm.u_normal_matrix, 1, GL_FALSE, glm::value_ptr(gm.normal_matrix));
    glUniformMatrix4fv(ugm.u_m_v_p_matrix, 1, GL_FALSE, glm::value_ptr(gm.proj_matrix * gm.view_matrix * gm.m_v_matrix));
}

void matrices_swan(GlobalMatrix &gm, uGlobalMatrix &ugm, Arpenteur player)
{
    gm.m_v_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(player.get_position()));
    gm.m_v_matrix = glm::scale(gm.m_v_matrix, glm::vec3(0.25,0.25,0.25));
    gm.normal_matrix = glm::transpose(glm::inverse(gm.m_v_matrix));
    glUniformMatrix4fv(ugm.u_m_v_matrix, 1, GL_FALSE, glm::value_ptr(gm.view_matrix * gm.m_v_matrix));
    glUniformMatrix4fv(ugm.u_normal_matrix, 1, GL_FALSE, glm::value_ptr(gm.normal_matrix));
    glUniformMatrix4fv(ugm.u_m_v_p_matrix, 1, GL_FALSE, glm::value_ptr(gm.proj_matrix * gm.view_matrix * gm.m_v_matrix));
}

void matrices_boids(GlobalMatrix &gm, uGlobalMatrix &ugm, glm::mat4 model_matrix) 
{
    gm.normal_matrix = glm::transpose(glm::inverse(model_matrix));
    
    glUniformMatrix4fv(ugm.u_normal_matrix, 1, GL_FALSE, glm::value_ptr(gm.normal_matrix));
    glUniformMatrix4fv(ugm.u_m_v_p_matrix, 1, GL_FALSE, glm::value_ptr(gm.proj_matrix));
}

void get_uniform_locations(const bool instancing, const p6::Shader &shader, uGlobalMatrix &ugm) 
{
    ugm.u_normal_matrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
    ugm.u_m_v_p_matrix = glGetUniformLocation(shader.id(), "uMVPMatrix");
    if (!instancing) {
        ugm.u_m_v_matrix = glGetUniformLocation(shader.id(), "uMVMatrix");
    }
}

glm::mat4 translate(float tx, float ty, float tz) {
    return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(tx, ty, tz, 1));
}

glm::mat4 scale(float sx, float sy, float sz) {
    return glm::mat4(glm::vec4(sx, 0, 0, 0), glm::vec4(0, sy, 0, 0), glm::vec4(0, 0, sz, 0), glm::vec4(0, 0, 0, 1));
}
