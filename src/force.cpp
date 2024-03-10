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
    if(glm::length(b.get_position() - self.get_position()) <= self.get_radius()) //Si l'autre boid suffisament proche
    {
        m_value = - m_multiplicator * (b.get_position() - self.get_position()) / (glm::length(b.get_position() - self.get_position()) * glm::length(b.get_position() - self.get_position()));//Force proportionelle à l'inverse de la distance entre les boids, dirigée dans l'autre sens de leur distance.
    }
    else m_value = glm::vec3(0,0,0);
}
void Force::alignement(Boid self, Boid b)
{
    if(glm::length(b.get_position() - self.get_position()) <= self.get_radius())
    {
        m_value = m_multiplicator * b.get_velocity(); // Pour aligner les deux boids on leur ajoute la vitesse de l'autre pour qu'ils tendent vers la meme direction
    }
    else m_value = glm::vec3(0,0,0);
}
 void Force::centering(Boid self, Boid b) 
{
    if(glm::length(b.get_position() - self.get_position()) <= self.get_radius())
    {
        m_value = m_multiplicator * ((b.get_position() + self.get_position()) * 0.5f - self.get_position()) / glm::length((b.get_position() + self.get_position()) * 0.5f - self.get_position()); //Vecteur normalisé du boid vers le milieu de la distance entre les deux boids, multiplié par l'intensité de la force.
    }
    else m_value = glm::vec3(0,0,0);
}