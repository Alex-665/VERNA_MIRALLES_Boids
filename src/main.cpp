#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "flock.hpp"
#include "include_glm.hpp"
#include "boid.hpp"
#include "force.hpp"
#include "loader.h"
#include <iostream>
#include "vao.hpp"
#include "vbo.hpp"
#include "matrix.hpp"
#include "texture.hpp"

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
        "src/shaders/texture.fs.glsl"
    );
    //ctx.maximize_window();
    
    // HERE IS THE INITIALIZATION CODE

    Object3D suzanne = loadOBJ("../models/suzanne.obj");
    Vbo vbo(2);
    vbo.gen();
    vbo.bind(0);
    glBufferData(GL_ARRAY_BUFFER, suzanne.vertices.size() * sizeof(vertex), suzanne.vertices.data(), GL_STATIC_DRAW);
    vbo.unbind();

    Vao vao;
    vao.gen();
    vao.bind();
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    //to enable the four vec4 of the instancematrix
    //glEnableVertexAttribArray(3);
    //glEnableVertexAttribArray(4);
    //glEnableVertexAttribArray(5);
    //glEnableVertexAttribArray(6);

    vbo.bind();
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, uv));
    //way for the vao to read the matrix of transformation
    //glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)offsetof(0 * glm::mat4));
    //glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)offsetof(1 * glm::mat4));
    //glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)offsetof(2 * glm::mat4));
    //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)offsetof(3 * glm::mat4));

    //glVertexAttribDivisor(3, 1);
    //glVertexAttribDivisor(4, 1);
    //glVertexAttribDivisor(5, 1);
    //glVertexAttribDivisor(6, 1);
    //
    vbo.unbind();
    vao.unbind();

    Flock flock = Flock();
    for(int i = 0 ; i < 25 ; i++)
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

    texture monkey("../textures/monkey.png");
    GLint uTexture = glGetUniformLocation(shader.id(), "uTexture");

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Almond);
        ctx.square(p6::Center{}, p6::Radius{1.f});
        gm.ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        vao.bind();
        glEnable(GL_CULL_FACE); //Hide the back faces of the model
        
        for(const auto& e : flock.get_boids())
        {
            shader.use();
            moveBoid(gm, ugm, e, ctx.time());
            glBindTexture(GL_TEXTURE_2D, monkey.texture_id);
            glUniform1i(uTexture, 0);
            glDrawArrays(GL_TRIANGLES, 0, suzanne.vertices.size());
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        flock.update(ctx.delta_time(), 1, params);
        vao.unbind();
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    //normalement ici les vbo et vao sont détruits automatiquement par les destructeurs désignés
}
