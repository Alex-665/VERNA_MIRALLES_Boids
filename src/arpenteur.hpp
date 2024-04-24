#pragma once
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Arpenteur
{
    private: 
        glm::vec3 m_position;
        float m_speed;
    public:
        Arpenteur():m_position(glm::vec3(0,0,0)), m_speed(1){};
        inline glm::vec3 get_position(){return m_position;};
        inline void move_front(float dt){m_position += glm::vec3(0,0,m_speed * dt);};
        inline void move_right(float dt){m_position += glm::vec3(m_speed * dt,0,0);};
        inline void move_up(float dt){m_position += glm::vec3(0,m_speed * dt,0);};
        inline void set_speed(float s){m_speed = s;};
};