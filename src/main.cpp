#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "flock.hpp"
#include "include_glm.hpp"
#include "boid.hpp"
#include "force.hpp"
#include "loader.h"
#include <iostream>
#include "vao.hpp"
#include "vbo.hpp"
#include "matrix.hpp"


int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code

    parameters params;

    auto ctx = p6::Context{{.width = 720, .height = 720, .title="Simple-p6-Setup"}};
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

    const p6::Shader shader = p6::load_shader(
        "src/shaders/red.vs.glsl",
        "src/shaders/3d.fs.glsl"
    );
    //ctx.maximize_window();
    
    // HERE IS THE INITIALIZATION CODE

    Object3D suzanne = loadOBJ("../models/suzanne.obj");
    Vbo vbo;
    vbo.gen();
    vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, suzanne.vertices.size() * sizeof(vertex), suzanne.vertices.data(), GL_STATIC_DRAW);
    vbo.unbind();

    Vao vao;
    vao.gen();
    vao.bind();
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    //const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    //glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    vbo.bind();
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, normal));
    vbo.unbind();
    vao.unbind();

    Flock flock = Flock();
    for(int i = 0 ; i < 50 ; i++)
    {
        Boid tmp;
        flock.add_boid(tmp);
    }
    Force avoidance(params._multiplicator_avoidance); //Tendance à augmenter la distance inter-boids
    Force alignement(params._multiplicator_alignement); //Tendance à former des gros groupes facilement
    Force centering(params._multiplicator_centering); //Tendance à diminuer le rayon d'un groupe de boid

    
    uGlobalMatrix ugm;
    getUniformLocations(shader, ugm);
    globalMatrix gm;


    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::PurpleHeart);
        ctx.square(p6::Center{}, p6::Radius{1.f});
        gm.ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        vao.bind();
        glEnable(GL_CULL_FACE); //Hide the back faces of the model
        
        for(const auto& e : flock.get_boids())
        {
            shader.use();
            moveBoid(gm, ugm, e, ctx.time());
            glDrawArrays(GL_TRIANGLES, 0, suzanne.vertices.size());
        }
        flock.update(ctx.delta_time(), 1, params);
        vao.unbind();
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    //normalement ici les vbo et vao sont détruits automatiquement par les destructeurs désignés
}
