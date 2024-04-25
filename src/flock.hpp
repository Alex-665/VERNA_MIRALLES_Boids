#pragma once

#include "p6/p6.h"
#include "force.hpp"
#include <vector>

struct Parameters{
    float _multiplicator_avoidance = 5.f;
    float _multiplicator_centering = 1.f;
    float _multiplicator_alignement = 3.f;
    int _boids_number = 50;  
};

class Flock
{
    private:
    std::vector<Boid> boids;
    float avoidance_multiplicator{10.f}; //Increase inter-boids gap
    float alignement_multiplicator{1.f}; //Form big groups of boids
    float centering_multiplicator{3.f}; //Shorten the diameter of a group of boids

    public:
    Flock(): boids(0){};
    Flock(int n): boids(n){};
    void add_boid(const Boid& b);
    void set_boid_number(int nb);
    void remove_boid();
    inline std::vector<Boid> get_boids(){return boids;};
    void update(float dt, float aspect_ratio, Parameters p);
    void set_avoidance_multiplicator(float m){avoidance_multiplicator = m;};
    void set_alignement_multiplicator(float m){alignement_multiplicator = m;};
    void set_centering_multiplicator(float m){centering_multiplicator = m;};
    private:
    void reset_acceleration();
    void accelerer(float dt, glm::vec3 force);
    void deplacer(float dt);
    void teleporter(float aspect_ratio);
    void move(float dt, float aspect_ratio);
    void compute_avoidance(Force &f);
    void compute_aligement(Force &f);
    void compute_centering(Force &f);
    glm::vec3 compute_forces(std::vector<Boid> boids, float avoidance_multiplicator, float alignement_multiplicator, float centering_multiplicator); //Run through all boids one time instead of one time per force
};
