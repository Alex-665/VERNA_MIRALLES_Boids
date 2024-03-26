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
    
    // HERE IS THE INITIALIZATION CODE
    GLfloat vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const GLuint VERTEX_ATTR_POSITION = 0;
    //const GLuint VERTEX_ATTR_NORMAL = 1;
    //const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    //glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    //glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
        glBindVertexArray(vao);
        //il récupère la matrice du dernier boid pour le suivre
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
