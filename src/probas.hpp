#pragma once
#include <cmath>
#include <cstdlib>
#include "flock.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/trigonometric.hpp"

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
inline double normal_law(float multiplicator){return multiplicator * glm::sqrt(-2*glm::log(rand01())) * glm::cos(2 * glm::pi<float>() * rand01());};
void computeEvolution(Flock &flock, const p6::Context &ctx, float &birth_time, float &death_time, int &n);
int poissonLaw(float lambda);
