#pragma once
#include "p6/p6.h"
#include "boid.hpp"

class Force {
    private:
        glm::vec3 m_value;
        float m_multiplicator = 1;
    public:
        Force() = default;
        Force (float m) : m_multiplicator(m) {};
        inline glm::vec3 get() {return m_value;};
        void invert(Boid self);
        void avoidance(Boid self, Boid b);
        void centering(Boid self, Boid b);
        void alignement(Boid self, Boid b);
        void all();
};

glm::vec3 invert(Boid self);
glm::vec3 avoidance(Boid self, Boid b, float multiplicator);
glm::vec3 alignement(Boid self, Boid b, float multiplicator);
glm::vec3 centering(Boid self, Boid b, float multiplicator);
