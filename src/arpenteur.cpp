#include "arpenteur.hpp"

void Arpenteur::move_front(float dt)
{
    if(dt > 0 && m_position.z < 44) m_position += glm::vec3(0,0,m_speed * dt);
    else if(dt < 0 && m_position.z > -44) m_position += glm::vec3(0,0,m_speed * dt);
}

void Arpenteur::move_right(float dt)
{
    if(dt > 0 && m_position.x < 44) m_position += glm::vec3(m_speed * dt,0,0);
    else if(dt < 0 && m_position.x > -44) m_position += glm::vec3(m_speed * dt,0,0);
}

void Arpenteur::move_up(float dt)
{
    if(dt > 0 && m_position.y < 44) m_position += glm::vec3(0,m_speed * dt,0);
    else if(dt < 0 && m_position.y > -44) m_position += glm::vec3(0,m_speed * dt,0);
}

void Arpenteur::move_third_person(const p6::Context &ctx, FreeflyCamera &camera)
{
    if (ctx.key_is_pressed(GLFW_KEY_LEFT)) {
        move_right(-ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_RIGHT)) {
        move_right(ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_UP)) {
        move_front(-ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_DOWN)) {
        move_front(ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_U)) {
        move_up(ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_D)) {
        move_up(-ctx.delta_time());
    }
    if(ctx.key_is_pressed(GLFW_KEY_KP_ADD)) {
        camera.add_distance(-1);
    }
    if(ctx.key_is_pressed(GLFW_KEY_KP_SUBTRACT)) {
        camera.add_distance(1);
    }
    if (ctx.mouse_button_is_pressed(p6::Button::Left)) {
        camera.rotate_left(-50.f * ctx.mouse_delta().x);
        camera.rotate_up(-50.f * ctx.mouse_delta().y);
    }
}

void Arpenteur::shape_state(const p6::Context &ctx, MarkovChain &mc, std::string &s)
{
    if(((int)(ctx.time()) % 5) == 0 && ((int)(ctx.time() -ctx.delta_time()) % 5) != 0) //On ne change d'état que toutes les 5 secondes
    {
        double random_uniform = rand01();
        if (random_uniform < mc.getState().x) 
        {   
            s = "En Forme";
            set_speed(20.f);
        }
        else if (random_uniform < mc.getState().x + mc.getState().y) 
        {
            s = "Fatigue Legere";
            set_speed(10.f);
        }
        else
        {
            s = "Fatigue Lourde";
            set_speed(5.f);
        }
        mc.nextState();
    }
}
