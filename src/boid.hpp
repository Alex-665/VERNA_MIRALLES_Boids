#pragma once
#include <cstddef>
#include <cstdlib>
#include "doctest/doctest.h"
#include "glm/glm.hpp"
#include <random>
#include "p6/p6.h"
#include <vector>

class Boid
{
    private:
    glm::vec3 m_position;
    glm::vec3 m_vitesse;
    glm::vec3 m_acceleration;
    glm::vec3 m_direction = {-1.f,0.f,0.f};
    float m_vitesse_max;
    float m_radius = 5.f;
    std::vector<GLfloat> vertex = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f
    };
    std::vector<Boid> m_voisins;

    public:
    Boid();
    Boid(glm::vec3 position, glm::vec3 vitesse, glm::vec3 acceleration, float vitesse_max) : m_position(position), m_vitesse(vitesse), m_acceleration(acceleration), m_vitesse_max(vitesse_max) {};
    glm::vec3 get_position() const;
    float get_radius() {return m_radius;};
    inline glm::vec3 get_velocity() {return m_vitesse;};
    inline std::vector<GLfloat> get_vertex() {return vertex;};
    inline glm::vec3 get_direction(){return m_direction;};
    void reset_acceleration();
    void ajouter_force(glm::vec3 force);
    void accelerer(float dt, glm::vec3 force);
    glm::vec3 compute_forces(std::vector<Boid> boids, float avoidance_multiplicator, float alignement_multiplicator, float centering_multiplicator);
    void deplacer(float dt);
    void teleporter(float aspect_ratio);
    void set_vitesse_max(const float vitesse_max);
    void update(float dt, float aspect_ratio, std::vector<Boid> boids, float avoidance_multiplicator, float alignement_multiplicator, float centering_multiplicator);
};
