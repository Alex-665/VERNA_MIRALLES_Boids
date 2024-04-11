#include "matrix.hpp"

void moveBoid(globalMatrix &gm, uGlobalMatrix &ugm, glm::mat4 model_matrix) 
{
    //gm.MVMatrix = translate(b.get_position().x, b.get_position().y, b.get_position().z);
    //gm.MVMatrix = gm.MVMatrix * scale(0.1f, 0.1f, 0.1f);
    //gm.MVMatrix = glm::rotate(gm.MVMatrix, time , glm::vec3(0,1,0));
    //glUniformMatrix4fv(ugm.uMVMatrix, 1, GL_FALSE, glm::value_ptr(gm.MVMatrix));
    gm.NormalMatrix = glm::transpose(glm::inverse(model_matrix));
    glUniformMatrix4fv(ugm.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(gm.NormalMatrix));
    glUniformMatrix4fv(ugm.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(gm.ProjMatrix));
}

void getUniformLocations(const p6::Shader &shader, uGlobalMatrix &ugm) 
{
    //ugm.uMVMatrix = glGetUniformLocation((shader).id(), "uMVMatrix");
    ugm.uMVPMatrix = glGetUniformLocation((shader).id(), "uMVPMatrix");
    ugm.uNormalMatrix = glGetUniformLocation((shader).id(), "uNormalMatrix");
}

glm::mat4 translate(float tx, float ty, float tz) {
    return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(tx, ty, tz, 1));
}

glm::mat4 scale(float sx, float sy, float sz) {
    return glm::mat4(glm::vec4(sx, 0, 0, 0), glm::vec4(0, sy, 0, 0), glm::vec4(0, 0, sz, 0), glm::vec4(0, 0, 0, 1));
}
