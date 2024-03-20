#include <imgui.h>
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

    parameters params;

    auto ctx = p6::Context{{.width = 1280, .height = 720, .title="Simple-p6-Setup"}};
    ctx.imgui = [&]() {
        ImGui::Begin("Caractéristiques");
        ImGui::SliderFloat("avoidance multiplicator", &params._multiplicator_avoidance, 0.01f, 10.f);
        ImGui::SliderFloat("centering multiplicator", &params._multiplicator_centering, 0.01f, 10.f);
        ImGui::SliderFloat("alignement multiplicator", &params._multiplicator_alignement, 0.01f, 10.f);
        ImGui::SliderInt("boids number", &params._boids_number, 1, 100);
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
    Force avoidance(params._multiplicator_avoidance); //Tendance à augmenter la distance inter-boids
    Force alignement(params._multiplicator_alignement); //Tendance à former des gros groupes facilement
    Force centering(params._multiplicator_centering); //Tendance à diminuer le rayon d'un groupe de boid

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
        flock.update(ctx.delta_time(), ctx.aspect_ratio(), params);
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
