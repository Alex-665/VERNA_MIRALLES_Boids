#include "freeflyCamera.hpp"

void FreeflyCamera::computeDirectonVectors()
{
    float cos_theta = glm::cos(m_theta);
    m_frontVector = glm::vec3(cos_theta * glm::sin(m_phi), glm::sin(m_theta), cos_theta * glm::cos(m_phi));
    float phiPi = m_phi + glm::pi<float>()/2;
    m_leftVector = glm::vec3(glm::sin(phiPi), 0.f, glm::cos(phiPi));
    m_upVector = glm::cross(m_frontVector, m_leftVector);
}

void FreeflyCamera::moveFront(float t)
{
    m_position += t * m_frontVector;
}

void FreeflyCamera::moveLeft(float t)
{
    m_position += t * m_leftVector;
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_phi += glm::radians(degrees);
    computeDirectonVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    m_theta += glm::radians(degrees);
    computeDirectonVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    glm::mat4 view = glm::lookAt(m_position, m_position + m_frontVector, m_upVector);
    return view;
}
