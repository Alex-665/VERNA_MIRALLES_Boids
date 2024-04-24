#pragma once
#include "p6/p6.h"
#include "include_glm.hpp"

class FreeflyCamera {
    private:
        glm::vec3 m_position;
        float m_phi;
        float m_theta;
        glm::vec3 m_frontVector;
        glm::vec3 m_leftVector;
        glm::vec3 m_upVector;
        void computeDirectonVectors();
    public:
        FreeflyCamera(): m_position(glm::vec3(0.f, 0.f, 0.f)), m_phi(glm::pi<float>()), m_theta(0.f) 
    {
        computeDirectonVectors();
    }
        void moveFront(float t);
        void moveLeft(float t);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);
        glm::mat4 getViewMatrix() const;
        inline glm::vec3 get_position() {return m_position;};
        inline glm::vec3 get_frontVector() {return m_frontVector;};
        inline glm::vec3 get_leftVector() {return m_leftVector;};
        inline glm::vec3 get_upVector() {return m_upVector;};
        inline float get_phi() {return m_phi;};
        inline float get_tetha() {return m_theta;};
};
