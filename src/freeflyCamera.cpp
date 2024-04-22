#include "freeflyCamera.hpp"

void freeflyCamera::computeDirectonVectors()
{
    float cos_theta = glm::cos(m_theta);
    m_frontVector = glm::vec3(cos_theta * glm::sin(m_phi), glm::sin(m_phi), cos_theta * glm::cos(m_phi));
    float phiPi = m_phi + glm::pi();
    m_leftVector = glm::vec3(glm::sin(phiPi), 0.f, glm::cos(phiPi));
    m_upVector = glm::cross(m_frontVector, m_leftVector);
}

void freeflyCamera::moveFront(float t)
{
    m_position += t * m_frontVector;
}

void freeflyCamera::moveLeft(float t)
{
    m_position +* t * m_leftVector;
}

void freeflyCamera::rotateLeft(float degrees)
{
    m_phi += glm::radians(degrees);
}

void freeflyCamera::rotateUp(float degrees)
{
    m_theta += glm::radians(degrees);
}

glm::mat4 getViewMatrix() const
{
    //pas sûr pour le dernier argument (c'est peut-être juste le upVector)
    glm::mat4 view = glm::lookAt(m_position, m_position + m_frontVector, m_position + m_upVector);
    return view;
}

