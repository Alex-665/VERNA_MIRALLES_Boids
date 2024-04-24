#include "arpenteur.hpp"

void Arpenteur::move_front(float dt)
{
    if(dt > 0 && m_position.z < 44) m_position += glm::vec3(0,0,m_speed * dt);
    else if(dt < 0 && m_position.z > -44) m_position += glm::vec3(0,0,m_speed * dt);
}

void Arpenteur::move_right(float dt)
{
    if(dt > 0 && m_position.x < 44) m_position += glm::vec3(m_speed * dt,0,0);
    else if(dt < 0 && m_position.x > -44) m_position += glm::vec3(m_speed * dt,0,0);
}

void Arpenteur::move_up(float dt)
{
    if(dt > 0 && m_position.y < 44) m_position += glm::vec3(0,m_speed * dt,0);
    else if(dt < 0 && m_position.y > -44) m_position += glm::vec3(0,m_speed * dt,0);
}