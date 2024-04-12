#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
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
        ImGui::SliderInt("boids number", &params._boids_number, 1, 1000);
        ImGui::End();
        ImGui::ShowDemoWindow();
    };
    srand(static_cast<unsigned int>(time(NULL))); // Initialize random seed

    const p6::Shader shader = p6::load_shader(
        "src/shaders/red.vs.glsl",
        "src/shaders/normals.fs.glsl"
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
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    vbo.bind(0);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, uv));
    vbo.unbind();
    //way for the vao to read the matrix of transformation
    vbo.bind(1);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(0 * sizeof(glm::vec4)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    //
    vbo.unbind();
    vao.unbind();

    Flock flock = Flock();
    for(int i = 0 ; i < params._boids_number ; i++)
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

        flock.update(ctx.delta_time(), 1, params);
        
        std::vector<glm::mat4> instanc_matrix(params._boids_number);
        std::vector<Boid> e = flock.get_boids();  
        for(size_t i = 0; i<params._boids_number; i++) {
            instanc_matrix[i] = translate(e[i].get_position().x, e[i].get_position().y, e[i].get_position().z);
            instanc_matrix[i] =  instanc_matrix[i] * scale(0.5f, 0.5f, 0.5f);
            instanc_matrix[i] = glm::rotate(instanc_matrix[i], -glm::degrees(glm::acos(glm::dot(glm::vec3(0,1,0), glm::vec3(0,0,1)))), glm::cross(glm::vec3(0,1,0), glm::vec3(0,0,1)));
        }
        //vbo.bind(1);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, params._boids_number * sizeof(glm::mat4), instanc_matrix.data());
        //vbo.unbind();
        //std::vector<glm::mat4> model_matrices(params._boids_number);

        vbo.bind(1);
        glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instanc_matrix.data(), GL_DYNAMIC_READ); 
        vbo.unbind();
        
        for(size_t i = 0; i<params._boids_number; i++)
        {
            shader.use();
            moveBoid(gm, ugm, instanc_matrix[i]);
            //glDrawArrays(GL_TRIANGLES, 0, suzanne.vertices.size());
        }
        glBindTexture(GL_TEXTURE_2D, monkey.texture_id);
        glUniform1i(uTexture, 0);
        vao.bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, suzanne.vertices.size(), params._boids_number);
        glBindTexture(GL_TEXTURE_2D, 0);
        vao.unbind();
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    //normalement ici les vbo et vao sont détruits automatiquement par les destructeurs désignés
}
