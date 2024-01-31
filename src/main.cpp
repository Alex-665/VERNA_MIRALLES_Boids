#include <cstddef>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"

class Boid
{
    private:
    glm::vec3 m_position ;
    glm::vec3 m_vitesse;
    glm::vec3 m_acceleration;

    public:
    Boid() : m_position(glm::vec3(0.f,0.f,0.f)), m_vitesse(glm::vec3(0., 0., 0.)), m_acceleration(glm::vec3(0.f,0.f,0.f)) {};
    Boid(glm::vec3 position, glm::vec3 vitesse, glm::vec3 acceleration) : m_position(position), m_vitesse(vitesse), m_acceleration(acceleration) {};
    glm::vec3 get_position() 
    {
        return m_position;
    }
    void ajouter_force(glm::vec3 force)
    {
        m_acceleration += force;
    }
    void accelerer(float dt)
    {
        m_vitesse += dt * m_acceleration;
    }
    void deplacer(float dt)
    {
        //rassembler comme pour accelerer
        m_position.x += dt * m_vitesse.x;
        m_position.y += dt * m_vitesse.y;
        m_position.z += dt * m_vitesse.z;
    }
    void teleporter(float aspect_ratio)
    {
        if(m_position.x > aspect_ratio || m_position.x < -aspect_ratio) m_position.x *= -1;
        if(m_position.y > 1 || m_position.y < -1) m_position.y *= -1;
    }     
};

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
    auto ctx = p6::Context{{WIDTH, HEIGHT, "Simple-p6-Setup"}};
    //ctx.maximize_window();
    Boid test_boid = Boid();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::PurpleHeart);
        ctx.circle(
            p6::Center{test_boid.get_position()},
            p6::Radius{0.2f}
        );
        test_boid.deplacer(ctx.delta_time());
        test_boid.teleporter(ctx.aspect_ratio());
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
