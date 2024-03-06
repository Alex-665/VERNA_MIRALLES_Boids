#include "boid.hpp"

// static constexpr vitesse_max = 1;
    float my_rand01()
    {
        float randomNumber = 2 * (float)rand() / (float)RAND_MAX - 1;
        return randomNumber;
    }
    Boid::Boid()
    {
        m_position = glm::vec3(my_rand01(), my_rand01(), 0);  
        m_vitesse =  glm::vec3(my_rand01(), my_rand01(), 0); 
        m_acceleration = glm::vec3(my_rand01(), my_rand01(), 0);
        m_vitesse_max = 1.f;
    }
    glm::vec3 Boid::get_position() 
    {
        return m_position;
    }
    void Boid::ajouter_force(glm::vec3 force)
    {
        m_acceleration += force;
    }
    void Boid::accelerer(float dt)
    {
        m_vitesse += dt * m_acceleration;
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
        if(m_position.x > aspect_ratio) m_position.x = -aspect_ratio;
        else if(m_position.x < -aspect_ratio) m_position.x = aspect_ratio;
        if(m_position.y > 1) m_position.y = -1;
        else if(m_position.y < -1) m_position.y = 1;
    }
    void Boid::set_vitesse_max(const float vitesse_max)
    {
        m_vitesse_max = vitesse_max;
    }
    void Boid::move(float dt, float aspect_ratio)
    {
        Boid::accelerer(dt);
        Boid::deplacer(dt);
        Boid::teleporter(aspect_ratio);
    }
    void Boid::reset_acceleration()
    {
        m_acceleration = glm::vec3(0,0,0);
    }