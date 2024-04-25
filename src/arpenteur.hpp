#pragma once
#include "glm/fwd.hpp"
#include "freeflyCamera.hpp"
#include "probas.hpp"

class Arpenteur
{
    private: 
        glm::vec3 m_position;
        float m_speed;
    public:
        Arpenteur():m_position(glm::vec3(0,0,5)), m_speed(20){};
        inline glm::vec3 get_position(){return m_position;};
        void move_front(float dt);
        void move_right(float dt);
        void move_up(float dt);
        inline void set_speed(float s){m_speed = s;};
        void move_third_person(const p6::Context &ctx, FreeflyCamera &camera);
        void shape_state(const p6::Context &ctx, MarkovChain &mc, std::string &s);
};
