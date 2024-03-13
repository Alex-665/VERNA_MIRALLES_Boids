#include <cstddef>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "flock.hpp"
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
    float multiplicator_avoidance = 1.f;
    float multiplicator_centering = 1.f;
    float multiplicator_alignement = 1.f;
    auto ctx = p6::Context{{WIDTH, HEIGHT, "Simple-p6-Setup"}};
    ctx.imgui = [&]() {
        ImGui::Begin("Caractéristiques");
        ImGui::SliderFloat("avoidance multiplicator", &multiplicator_avoidance, 0.01f, 10.f);
        ImGui::SliderFloat("centering multiplicator", &multiplicator_centering, 0.01f, 10.f);
        ImGui::SliderFloat("alignement multiplicator", &multiplicator_alignement, 0.01f, 10.f);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };
    srand(static_cast<unsigned int>(time(NULL))); // Initialize random seed
    //ctx.maximize_window();
    
    Flock flock = Flock();
    for(int i = 0 ; i < 50 ; i++)
    {
        Boid tmp;
        flock.add_boid(tmp);
    }
    Force avoidance(multiplicator_avoidance); //Tendance à augmenter la distance inter-boids
    Force alignement(multiplicator_alignement); //Tendance à former des gros groupes facilement
    Force centering(multiplicator_centering); //Tendance à diminuer le rayon d'un groupe de boid

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::PurpleHeart);
        
        for(const auto& e : flock.get_boids())
        {
            ctx.circle(
                p6::Center{e.get_position()},
                p6::Radius{0.02f}
            );
        }
        flock.reset_acceleration();
        Force avoidance(multiplicator_avoidance); //Tendance à augmenter la distance inter-boids
        Force alignement(multiplicator_alignement); //Tendance à former des gros groupes facilement
        Force centering(multiplicator_centering); //Tendance à diminuer le rayon d'un groupe de boid
        flock.compute_forces(avoidance, alignement, centering);
        flock.move(ctx.delta_time(), ctx.aspect_ratio());
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
