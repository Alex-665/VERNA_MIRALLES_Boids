#include "flock.hpp"
#include <regex>
#include "boid.hpp"

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
   void Flock::compute_force(Force &f)
   {
    for(auto &b : this->boids)
    {
        for(const auto &o : this->boids)
        {
            if(&b != &o)
            {
                f.avoidance(b, o);
                b.ajouter_force(f.get());
            }
        }
    }
   }