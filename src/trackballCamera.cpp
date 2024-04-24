#include "trackballCamera.hpp"

glm::mat4 TrackballCamera::getViewMatrix() const {
    float a_x = glm::cos(m_angleX);
    //glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(m_distance * a_x * glm::sin(m_angleY), m_distance * glm::sin(m_angleX), -m_distance * a_x * glm::cos(m_angleY)));
    glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -m_distance));
    ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_angleX), glm::vec3(1.f, 0.f, 0.f));
    ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_angleY), glm::vec3(0.f, 1.f, 0.f));
    return ViewMatrix;
}
