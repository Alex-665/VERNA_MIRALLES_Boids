#include "force.hpp"
#include "glm/fwd.hpp"

void Force::invert(Boid self) 
{
    glm::vec3 velocity = self.get_velocity();
    m_value.x = -2*velocity.x;
    m_value.y = -2*velocity.y;
    m_value.z = -2*velocity.z;
}
void Force::avoidance(Boid self, Boid b)
{
    if(glm::length(b.get_position() - self.get_position()) <= self.get_radius())
    {
        m_value = - m_multiplicator * (b.get_position() - self.get_position()) / (glm::length(b.get_position() - self.get_position()) * glm::length(b.get_position() - self.get_position()));
    }
    else m_value = glm::vec3(0,0,0);
}