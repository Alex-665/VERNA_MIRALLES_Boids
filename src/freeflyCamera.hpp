#pragma once
#include "p6/p6.h"
#include "include_glm.hpp"

class FreeflyCamera {
    private:
        float m_distance{10.f};
        float m_phi;
        float m_theta;
        glm::vec3 m_front_vector;
        glm::vec3 m_left_vector;
        glm::vec3 m_up_vector;
        void compute_direction_vectors();
    public:
        FreeflyCamera(): m_phi(0.f), m_theta(0.f) 
    {
        compute_direction_vectors();
    }
        void rotate_left(float degrees);
        void rotate_up(float degrees);
        glm::mat4 get_view_matrix(glm::vec3 arpenteur_position) const;
        inline glm::vec3 get_front_vector() {return m_front_vector;};
        inline glm::vec3 get_left_vector() {return m_left_vector;};
        inline glm::vec3 get_up_vector() {return m_up_vector;};
        inline float get_phi() {return m_phi;};
        inline float get_tetha() {return m_theta;};
};
