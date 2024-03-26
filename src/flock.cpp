#include "flock.hpp"
#include <regex>
#include "boid.hpp"
#include "glm/fwd.hpp"

// static constexpr vitesse_max = 1;

   void Flock::add_boid(const Boid& b)
   {
        boids.push_back(b);
   }

   void Flock::remove_boid()
   {
    if(!boids.empty())
    {
        boids.pop_back();
    }
   }
   void Flock::set_boid_number(int nb)
   {
        if(this->boids.size() < nb)
        {
            for(int i = 0 ; i < this->boids.size() - nb ; i++)
            {
                const Boid b;
                this->add_boid(b);
            }
        }
        else
        {
            for(int i = 0; i < this->boids.size() - nb ; i++)
            {
                this->remove_boid();
            }
        }
   }
//    void Flock::reset_acceleration()
//    {
//     for(auto &e : boids)
//     {
//         e.reset_acceleration();
//     }
//    }
   void Flock::accelerer(float dt, glm::vec3 force)
   {
        for(auto &e : boids)
        {
            e.accelerer(dt, force);
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
            e.update(dt, aspect_ratio, boids, avoidance_multiplicator, alignement_multiplicator, centering_multiplicator);
        }
   }
//    void Flock::compute_avoidance(Force &f)
//    {
//     int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
//         glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
//         for(auto &b : this->boids)
//         {
//             for(const auto &o : this->boids)
//             {
//                 if(&b != &o)//Le boid ne se soucie pas de lui même
//                 {
//                     f.avoidance(b, o);
//                     if(glm::length(f.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
//                     {
//                         neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
//                         tmp += f.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
//                     } 
//                 }
//             }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
//             b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
//             neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
//             tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
//         }
//    }
//    void Flock::compute_aligement(Force &f)
//    {
//         int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
//         glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
//         for(auto &b : this->boids)
//         {
//             for(const auto &o : this->boids)
//             {
//                 if(&b != &o)//Le boid ne se soucie pas de lui même
//                 {
//                     f.alignement(b, o);
//                     if(glm::length(f.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
//                     {
//                         neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
//                         tmp += f.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
//                     } 
//                 }
//             }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
//             b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
//             neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
//             tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
//         }
//    }
//    void Flock::compute_centering(Force &f)
//    {
//         int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
//         glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
//         for(auto &b : this->boids)
//         {
//             for(const auto &o : this->boids)
//             {
//                 if(&b != &o)//Le boid ne se soucie pas de lui même
//                 {
//                     f.centering(b, o);
//                     if(glm::length(f.get()) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
//                     {
//                         neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
//                         tmp += f.get(); //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
//                     } 
//                 }
//             }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
//             b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
//             neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
//             tmp = glm::vec3(0.f,0.f,0.f); //On reset le vecteur temporaire pour le boid suivant.
//         }
//    }
//     glm::vec3 compute_forces(std::vector<Boid> boids, float avoidance_multiplicator, float alignement_multiplicator, float centering_multiplicator) //Cette fonction nous permet de parcourir tous les boids 1 fois en tout au lieu de 1 fois par force
//    {
//         int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
//         glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
//         for(auto &b : boids)
//         {
//             for(const auto &o : boids)
//             {
//                 if(&b != &o)//Le boid ne se soucie pas de lui même
//                 {
//                     glm::vec3 force{};
//                     force += avoidance(b, o, avoidance_multiplicator);
//                     force += alignement(b, o, alignement_multiplicator);
//                     force += centering(b, o, centering_multiplicator);
//                     if(glm::length(force) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
//                     {
//                         neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
//                         tmp += force; //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
//                     } 
//                 }
//             }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
//             b.ajouter_force(tmp); //On ajoute au boid la version pondérée de la force.
//             neighbour_nb = 0; //On reset le nombre de voisins pour le boid suivant.
//             return tmp;
            
//         }
//    }

   void Flock::update(float dt, float aspect_ratio, parameters p){
    
    // reset_acceleration();
    move(dt, aspect_ratio);
    set_avoidance_multiplicator(p._multiplicator_avoidance);
    set_alignement_multiplicator(p._multiplicator_alignement);
    set_centering_multiplicator(p._multiplicator_centering);
    set_boid_number(p._boids_number);
   }