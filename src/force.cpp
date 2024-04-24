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
    if(glm::distance(b.get_position(), self.get_position()) <= self.get_radius()) //Si l'autre boid suffisament proche
    {
        return - multiplicator * (b.get_position() - self.get_position()) / (glm::distance(b.get_position(), self.get_position()) * glm::distance(b.get_position(), self.get_position()));//Force proportionelle à l'inverse de la distance entre les boids, dirigée dans l'autre sens de leur distance.
    }
    return glm::vec3{0};
}
glm::vec3 alignement(Boid self, Boid b, float multiplicator)
{
    if(glm::distance(b.get_position(), self.get_position()) <= self.get_radius())
    {
        return multiplicator * b.get_velocity(); // Pour aligner les deux boids on leur ajoute la vitesse de l'autre pour qu'ils tendent vers la meme direction
    }
    return glm::vec3{0};
}
glm::vec3 centering(Boid self, Boid b, float multiplicator) 
{
    if(glm::distance(b.get_position(), self.get_position()) <= self.get_radius())
    {
        return multiplicator * ((b.get_position() + self.get_position()) * 0.5f - self.get_position()) / glm::distance((b.get_position() + self.get_position()) * 0.5f, self.get_position()); //Vecteur normalisé du boid vers le milieu de la distance entre les deux boids, multiplié par l'intensité de la force.
    }
    return glm::vec3{0};
}
