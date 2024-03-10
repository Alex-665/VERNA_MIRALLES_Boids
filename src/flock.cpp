#include "flock.hpp"
#include <regex>
#include "boid.hpp"
#include "glm/fwd.hpp"

// static constexpr vitesse_max = 1;

   void Flock::add_boid(Boid& b)
   {
        boids.push_back(b);
        m_number ++;
   }

   void Flock::remove_boid()
   {
    if(m_number > 0)
    {
        boids.pop_back();
        m_number --;
    }
   }
   void Flock::reset_acceleration()
   {
    for(auto &e : boids)
    {
        e.reset_acceleration();
    }
   }
   void Flock::accelerer(float dt)
   {
        for(auto &e : boids)
        {
            e.accelerer(dt);
        }
   }
   void Flock::deplacer(float dt)
   {
        for(auto &e : boids)
        {
            e.deplacer(dt);
        }
   }
   void Flock::teleporter(float aspect_ratio)
   {
        for(auto &e : boids)
        {
            e.teleporter(aspect_ratio);
        }
   }
   void Flock::move(float dt, float aspect_ratio)
   {
        for(auto &e : boids)
        {
            e.move(dt, aspect_ratio);
        }
   }
   void Flock::compute_avoidance(Force &f)
   {
    int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
        glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
        for(auto &b : this->boids)
        {
            for(const auto &o : this->boids)
            {
                if(&b != &o)//Le boid ne se soucie pas de lui même
                {
                    f.avoidance(b, o);
                    if(glm::length(f.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
                    {
                        neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
                        tmp += f.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
                    } 
                }
            }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
            b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
            neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
            tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
        }
   }
   void Flock::compute_aligement(Force &f)
   {
        int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
        glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
        for(auto &b : this->boids)
        {
            for(const auto &o : this->boids)
            {
                if(&b != &o)//Le boid ne se soucie pas de lui même
                {
                    f.alignement(b, o);
                    if(glm::length(f.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
                    {
                        neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
                        tmp += f.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
                    } 
                }
            }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
            b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
            neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
            tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
        }
   }
   void Flock::compute_centering(Force &f)
   {
        int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
        glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
        for(auto &b : this->boids)
        {
            for(const auto &o : this->boids)
            {
                if(&b != &o)//Le boid ne se soucie pas de lui même
                {
                    f.centering(b, o);
                    if(glm::length(f.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
                    {
                        neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
                        tmp += f.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
                    } 
                }
            }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
            b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
            neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
            tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
        }
   }
   void Flock::compute_forces(Force & avoidance, Force & alignement, Force & centering) //Cette fonction nous permet de parcourir tous les boids 1 fois en tout au lieu de 1 fois par force
   {
        int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
        glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
        for(auto &b : this->boids)
        {
            for(const auto &o : this->boids)
            {
                if(&b != &o)//Le boid ne se soucie pas de lui même
                {
                    avoidance.avoidance(b, o);
                    alignement.alignement(b, o);
                    centering.centering(b, o);
                    if(glm::length(avoidance.get() + alignement.get() + centering.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
                    {
                        neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
                        tmp += avoidance.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
                        tmp += alignement.get();
                        tmp += centering.get();
                    } 
                }
            }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
            b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
            neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
            tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
        }
   }