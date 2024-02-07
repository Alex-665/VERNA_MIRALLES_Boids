#include "force.hpp"

void Force::invert(Boid self) 
{
    glm::vec3 velocity = self.get_velocity();
    m_value.x = -2*velocity.x;
    m_value.y = -2*velocity.y;
    m_value.z = -2*velocity.z;
}