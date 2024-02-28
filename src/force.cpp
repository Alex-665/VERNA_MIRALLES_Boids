#include "force.hpp"

void Force::invert(Boid self) 
{
    glm::vec3 velocity = self.get_velocity();
    m_value.x = -2*velocity.x;
    m_value.y = -2*velocity.y;
    m_value.z = -2*velocity.z;
}
void Force::avoidance(Boid self, Boid b)
{
    if(glm::length(b.get_position() - self.get_position()) <= self.get_radius()){
        if(glm::length(b.get_position() - self.get_position()) > 0.01)
        {
            m_value.x = -1.f/(b.get_position().x - self.get_position().x);
            m_value.y = -1.f/(b.get_position().y - self.get_position().y);
            m_value.z = -1.f/(b.get_position().z - self.get_position().z);
        }
        
    }
}