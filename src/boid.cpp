#include "boid.hpp"
#include "probas.hpp"
#include "force.hpp"

// static constexpr vitesse_max = 1;
    Boid::Boid()
    {
        m_position = glm::vec3(normal_law(10), normal_law(10), normal_law(10));  
        m_vitesse =  glm::vec3(rand01(), rand01(), rand01()); 
        m_acceleration = glm::vec3(rand01(), rand01(), rand01());
        m_vitesse_max = 10.f;
    }
    glm::vec3 Boid::get_position() const
    {
        return m_position;
    }
    void Boid::accelerer(float dt, glm::vec3 force)
    {
        m_vitesse += dt * force;
        if(glm::length(m_vitesse) > m_vitesse_max )
        {
            m_vitesse = m_vitesse * (m_vitesse_max / glm::length(m_vitesse));
        }
    }
    void Boid::deplacer(float dt)
    {
        m_position += dt * m_vitesse;
    }
    void Boid::teleporter(float aspect_ratio)
    {
        if(m_position.x > 45) m_position.x = -45;
        else if(m_position.x < -45) m_position.x = 45;
        if(m_position.y > 45) m_position.y = -45;
        else if(m_position.y < -45) m_position.y = 45;
        //rajout pour la coordonnée en z (en attendant d'avoir une bonne caméra)
        if(m_position.z > 45) m_position.z = -45;
        else if(m_position.z < -45) m_position.z = 45;
    }
    void Boid::set_vitesse_max(const float vitesse_max)
    {
        m_vitesse_max = vitesse_max;
    }
    void Boid::update(float dt, float aspect_ratio, std::vector<Boid> boids, float avoidance_multiplicator, float alignement_multiplicator, float centering_multiplicator)
    {
        glm::vec3 force = compute_forces(boids, avoidance_multiplicator, alignement_multiplicator, centering_multiplicator);
        Boid::accelerer(dt, force);
        Boid::deplacer(dt);
        Boid::teleporter(aspect_ratio);
    }
    glm::vec3 Boid::compute_forces(std::vector<Boid> boids, float avoidance_multiplicator, float alignement_multiplicator, float centering_multiplicator)
    {
        int neighbour_nb = 0; //Pour pondérer la force d'alignement par le nombre de boids
        glm::vec3 tmp = glm::vec3(0.f,0.f,0.f); //Pour contenir la version pondérée de la force
            for(const auto &o : boids)
            {
                if(this != &o)//Le boid ne se soucie pas de lui même
                {
                    glm::vec3 force{};
                    force += avoidance(*this, o, avoidance_multiplicator);
                    force += alignement(*this, o, alignement_multiplicator);
                    force += centering(*this, o, centering_multiplicator);
                    if(glm::length(force) > 0) //La force n'aura une norme positive que si le voisin était suffisament proche
                    {
                        neighbour_nb ++; //On ajoute 1 au facteur de pondération pour faire la moyenne à la fin
                        tmp += force; //On ajoute au vecteur temporaire la valeur de la force calculée précdemment
                    } 
                }
            }if (neighbour_nb != 0) tmp *= 1/static_cast<float>(neighbour_nb); //On pondère la force par le nombre de voisins rencontrés
            return tmp;
    }
