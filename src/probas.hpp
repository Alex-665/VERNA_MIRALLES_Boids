#pragma once
#include <cmath>
#include <cstdlib>
#include "flock.hpp"
#include "glm/ext/scalar_constants.hpp"

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
inline double exponentialLaw(float lambda) {return (-1.f / lambda) * std::log(rand01());};
inline double normalLaw(float mean, float delta){return (1.f / (delta*std::sqrt(2*glm::pi<float>()))) * std::exp(-0.5 * std::pow(((rand01()-mean)/delta),2));};
void computeEvolution(Flock &flock, const p6::Context &ctx, float &birth_time, float &death_time, int &n);
int poissonLaw(float lambda);
