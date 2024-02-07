#include <cstddef>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "boid.hpp"
#include "force.hpp"

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
    Boid boid1 = Boid();
    Boid boid2 = Boid(glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
    boid1.set_vitesse_max(1.);
    glm::vec3 force(1., 0., 0.);
    boid1.ajouter_force(force);
    
    Force force2;
    force2.invert(boid2);
    boid2.ajouter_force(force2.get());

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::PurpleHeart);
        ctx.circle(
            p6::Center{boid1.get_position()},
            p6::Radius{0.05f}
        );
        ctx.circle(
            p6::Center{boid2.get_position()},
            p6::Radius{0.05f}
        );
        boid1.move(ctx.delta_time(), ctx.aspect_ratio());
        boid2.move(ctx.delta_time(), ctx.aspect_ratio());
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
