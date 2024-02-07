#pragma once
#include "p6/p6.h"
#include "boid.hpp"

class Force {
    private:
        glm::vec3 m_value;
    public:
        Force() = default;
        inline glm::vec3 get() {return m_value;};
        void invert(Boid self);
        void avoidance(Boid self, Boid b);
        void centering(Boid self, Boid b);
        void alignement(Boid self, Boid b);
        void all();
};