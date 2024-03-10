#pragma once

#include "doctest/doctest.h"
#include "glm/fwd.hpp"
#include "boid.hpp"
#include "p6/p6.h"
#include "force.hpp"
#include <vector>

class Flock
{
    private:
    int m_number = 0;
    std::vector<Boid> boids;

    public:
    Flock(): boids(m_number){};
    Flock(int n): m_number(n), boids(n){};
    void add_boid(Boid& b);
    void remove_boid();
    void reset_acceleration();
    void accelerer(float dt);
    void deplacer(float dt);
    void teleporter(float aspect_ratio);
    void move(float dt, float aspect_ratio);
    void compute_force(Force &f);
    inline std::vector<Boid> get_boids(){return boids;};
};