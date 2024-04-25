#include "freeflyCamera.hpp"

void FreeflyCamera::compute_direction_vectors()
{
    float cos_theta = glm::cos(m_theta);
    m_front_vector = glm::vec3(cos_theta * glm::sin(m_phi), glm::sin(m_theta), cos_theta * glm::cos(m_phi));
    float phiPi = m_phi + glm::pi<float>()/2;
    m_left_vector = glm::vec3(glm::sin(phiPi), 0.f, glm::cos(phiPi));
    m_up_vector = glm::cross(m_front_vector, m_left_vector);
}

void FreeflyCamera::rotate_left(float degrees)
{
    m_phi += glm::radians(degrees);
    compute_direction_vectors();
}

void FreeflyCamera::rotate_up(float degrees)
{
    m_theta += glm::radians(degrees);
    compute_direction_vectors();
}

glm::mat4 FreeflyCamera::get_view_matrix(glm::vec3 arpenteur_position) const
{
    glm::mat4 view = glm::lookAt(arpenteur_position + m_distance * m_front_vector, arpenteur_position, m_up_vector);
    return view;
}
