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
<<<<<<< Updated upstream
        "src/shaders/3d.fs.glsl"
=======
        "src/shaders/point_light.fs.glsl"
    );

    const p6::Shader draw_shader = p6::load_shader(
        "src/shaders/classic.vs.glsl",
        "src/shaders/point_light.fs.glsl"
>>>>>>> Stashed changes
    );
    //ctx.maximize_window();
    
    // HERE IS THE INITIALIZATION CODE

<<<<<<< Updated upstream
    Object3D suzanne = loadOBJ("../models/suzanne.obj");
    Vbo vbo;
    vbo.gen();
    vbo.bind();
=======
    Object3D suzanne = loadOBJ("../models/shark_high.obj");
    Vbo boids_vbo(2);
    boids_vbo.gen();
    boids_vbo.bind(0);
>>>>>>> Stashed changes
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


    GLint uKd = glGetUniformLocation(shader.id(), "uKd");
    GLint uKs = glGetUniformLocation(shader.id(), "uKs");
    GLint uShininess= glGetUniformLocation(shader.id(), "uShininess");
    GLint uLightPos_vs= glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLint uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");

    // Declare your infinite update loop.
    ctx.update = [&]() {
<<<<<<< Updated upstream
        ctx.background(p6::NamedColor::PurpleHeart);
=======
        ctx.background(p6::Color(0.2,0.4,0.6));
>>>>>>> Stashed changes
        ctx.square(p6::Center{}, p6::Radius{1.f});
        gm.ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

<<<<<<< Updated upstream
=======
        glEnable(GL_CULL_FACE); //Hide the back faces of the model
        glEnable(GL_DEPTH_TEST); //Checks if the fragment has to be rendered based on it's z value

        //Drawing the cube
        draw_shader.use();
        gm.MVMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
        gm.MVMatrix = glm::scale(gm.MVMatrix, glm::vec3(5,5,5));
        gm.NormalMatrix = glm::transpose(glm::inverse(gm.MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(gm.MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(gm.NormalMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(gm.ProjMatrix * gm.MVMatrix));

        glUniform3fv(uKd, 1, glm::value_ptr(glm::vec3(1,1,1)));
        glUniform3fv(uKs, 1, glm::value_ptr(glm::vec3(1,1,1)));
        glUniform1f(uShininess, 1);
        glUniform3fv(uLightPos_vs, 1, glm::value_ptr(glm::vec3(0,0,0)));
        glUniform3fv(uLightIntensity, 1, glm::value_ptr(glm::vec3(50,50,50)));

        glBindTexture(GL_TEXTURE_2D, cube_texture.texture_id); 
        glUniform1i(uTexture, 0);
        cube_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, cube.vertices.size());
        glBindTexture(GL_TEXTURE_2D, 0);
        cube_vao.unbind();

        flock.update(ctx.delta_time(), 1, params);
        
        std::vector<glm::mat4> instanc_matrix(params._boids_number);
        std::vector<Boid> e = flock.get_boids();  
        for(size_t i = 0; i<params._boids_number; i++) {
            instanc_matrix[i] = translate(e[i].get_position().x, e[i].get_position().y, e[i].get_position().z);
            instanc_matrix[i] =  instanc_matrix[i] * scale(0.5f, 0.5f, 0.5f);
            //instanc_matrix[i] = glm::rotate(instanc_matrix[i], glm::degrees(glm::acos(glm::dot(e[i].get_direction(), glm::normalize(e[i].get_velocity())))), glm::cross(e[i].get_direction(), glm::normalize(e[i].get_velocity())));
        }
        //boids_vbo.bind(1);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, params._boids_number * sizeof(glm::mat4), instanc_matrix.data());
        //boids_vbo.unbind();
        //std::vector<glm::mat4> model_matrices(params._boids_number);

        boids_vbo.bind(1);
        glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instanc_matrix.data(), GL_DYNAMIC_READ); 
        boids_vbo.unbind();

        shader.use();
        
        for(size_t i = 0; i<params._boids_number; i++)
        {
            moveBoid(gm, ugm, instanc_matrix[i]);
            //glDrawArrays(GL_TRIANGLES, 0, suzanne.vertices.size());
        }
        glBindTexture(GL_TEXTURE_2D, boids_texture.texture_id);
        glUniform1i(uTexture, 0);
        glUniform3fv(uKd, 1, glm::value_ptr(glm::vec3(1,1,1)));
        glUniform3fv(uKs, 1, glm::value_ptr(glm::vec3(1,1,1)));
        glUniform1f(uShininess, 1);
        glUniform3fv(uLightPos_vs, 1, glm::value_ptr(glm::vec3(0,0,0)));
        glUniform3fv(uLightIntensity, 1, glm::value_ptr(glm::vec3(5,5,5)));
>>>>>>> Stashed changes
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
