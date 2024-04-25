#include "force.hpp"
#include "glm/glm.hpp"

glm::vec3 invert(Boid self) 
{
    glm::vec3 value{};
    glm::vec3 velocity = self.get_velocity();
    value.x = -2*velocity.x;
    value.y = -2*velocity.y;
    value.z = -2*velocity.z;
    return value;
}

glm::vec3 avoidance(Boid self, Boid b, float multiplicator)
{
    if(glm::distance(b.get_position(), self.get_position()) <= self.get_radius())
    {
        return - multiplicator * (b.get_position() - self.get_position()) / (glm::distance(b.get_position(), self.get_position()) * glm::distance(b.get_position(), self.get_position()));
    }
    return glm::vec3{0};
}
glm::vec3 alignement(Boid self, Boid b, float multiplicator)
{
    if(glm::distance(b.get_position(), self.get_position()) <= self.get_radius())
    {
        return multiplicator * b.get_velocity();
    }
    return glm::vec3{0};
}
glm::vec3 centering(Boid self, Boid b, float multiplicator) 
{
    if(glm::distance(b.get_position(), self.get_position()) <= self.get_radius())
    {
        return multiplicator * ((b.get_position() + self.get_position()) * 0.5f - self.get_position()) / glm::distance((b.get_position() + self.get_position()) * 0.5f, self.get_position());
    }
    return glm::vec3{0};
}
