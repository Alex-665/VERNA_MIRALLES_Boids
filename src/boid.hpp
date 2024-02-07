#pragma once
#include <cstddef>
#include <cstdlib>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boid
{
    private:
    glm::vec3 m_position;
    glm::vec3 m_vitesse;
    glm::vec3 m_acceleration;
    float m_vitesse_max;
    float radius;

    public:
    Boid() : m_position(glm::vec3(0.f,0.f,0.f)), m_vitesse(glm::vec3(0., 0., 0.)), m_acceleration(glm::vec3(0.f,0.f,0.f)), m_vitesse_max{0} {};
    Boid(glm::vec3 position, glm::vec3 vitesse, glm::vec3 acceleration, float vitesse_max) : m_position(position), m_vitesse(vitesse), m_acceleration(acceleration), m_vitesse_max(vitesse_max) {};
    glm::vec3 get_position();
    inline glm::vec3 get_velocity() {return m_vitesse;};
    void ajouter_force(glm::vec3 force);
    void accelerer(float dt);
    void deplacer(float dt);
    void teleporter(float aspect_ratio);
    void set_vitesse_max(const float vitesse_max);
    void move(float dt, float aspect_ratio);
};