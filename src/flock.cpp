#include <regex>
#include "flock.hpp"
#include "boid.hpp"
#include "glm/fwd.hpp"

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

void Flock::update(float dt, float aspect_ratio, Parameters p){

    set_boid_number(p._boids_number);
    move(dt, aspect_ratio);
    set_avoidance_multiplicator(p._multiplicator_avoidance);
    set_alignement_multiplicator(p._multiplicator_alignement);
    set_centering_multiplicator(p._multiplicator_centering);
}
