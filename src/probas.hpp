#pragma once
#include <cmath>
#include "p6/p6.h"
#include "flock.hpp"

double rand01();

class MarkovChain {
    private:
        glm::vec3 m_state;
        glm::mat3 m_transitionMatrix;
    public:
        MarkovChain() = default;
        MarkovChain(glm::vec3 s, glm::mat3 m): m_state(s), m_transitionMatrix(m) {};
        inline void nextState() {m_state = m_transitionMatrix * m_state;};
        inline glm::vec3 getState() {return m_state;};
};

inline double uniformLaw() {return rand01();};
inline double exponentialLaw(float lambda) {return (-1 / lambda) * std::log(rand01());};
void computeEvolution(Flock &flock, const p6::Context &ctx, float &birth_time, float &death_time, parameters &p);
int poissonLaw(float lambda);