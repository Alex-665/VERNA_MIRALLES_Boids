#include "matrix.hpp"

void matricesCube(globalMatrix &gm, uGlobalMatrix &ugm)
{
    gm.MVMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
    gm.MVMatrix = glm::scale(gm.MVMatrix, glm::vec3(15,15,15));
    gm.NormalMatrix = glm::transpose(glm::inverse(gm.MVMatrix));
    glUniformMatrix4fv(ugm.uMVMatrix, 1, GL_FALSE, glm::value_ptr(gm.ViewMatrix * gm.MVMatrix));
    glUniformMatrix4fv(ugm.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(gm.NormalMatrix));
    glUniformMatrix4fv(ugm.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(gm.ProjMatrix * gm.ViewMatrix * gm.MVMatrix));
}

void matricesBoids(globalMatrix &gm, uGlobalMatrix &ugm, glm::mat4 model_matrix) 
{
    gm.NormalMatrix = glm::transpose(glm::inverse(model_matrix));
    
    glUniformMatrix4fv(ugm.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(gm.NormalMatrix));
    glUniformMatrix4fv(ugm.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(gm.ProjMatrix * gm.ViewMatrix));
}

void getUniformLocations(const bool instancing, const p6::Shader &shader, uGlobalMatrix &ugm) 
{
    ugm.uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
    ugm.uMVPMatrix = glGetUniformLocation(shader.id(), "uMVPMatrix");
    if (!instancing) {
        ugm.uMVMatrix = glGetUniformLocation(shader.id(), "uMVMatrix");
    }
}

glm::mat4 translate(float tx, float ty, float tz) {
    return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(tx, ty, tz, 1));
}

glm::mat4 scale(float sx, float sy, float sz) {
    return glm::mat4(glm::vec4(sx, 0, 0, 0), glm::vec4(0, sy, 0, 0), glm::vec4(0, 0, sz, 0), glm::vec4(0, 0, 0, 1));
}
