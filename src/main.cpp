#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
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
#include "utils.hpp"
#include "light.hpp"

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

    const p6::Shader shader(std::string("#version 330 core\n") + std::string("#define INSTANCING\n") + file_content("src/shaders/red.vs.glsl"), file_content("src/shaders/point_light.fs.glsl"));

    const p6::Shader draw_shader(std::string("#version 330 core\n") + file_content("src/shaders/red.vs.glsl"), file_content("src/shaders/point_light.fs.glsl"));
    
    // HERE IS THE INITIALIZATION CODE

    Object3D suzanne = loadOBJ("../models/shark.obj");
    Vbo boids_vbo(2);
    boids_vbo.gen();
    boids_vbo.bind(0);
    glBufferData(GL_ARRAY_BUFFER, suzanne.vertices.size() * sizeof(vertex), suzanne.vertices.data(), GL_STATIC_DRAW);
    boids_vbo.unbind();

    Object3D cube = loadOBJ("../models/cube.obj");
    Vbo cube_vbo(1);
    cube_vbo.gen();
    cube_vbo.bind(0);
    glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(vertex), cube.vertices.data(), GL_STATIC_DRAW);

    Vao vao(1);
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

    boids_vbo.bind(0);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, uv));
    boids_vbo.unbind();
    //way for the vao to read the matrix of transformation
    boids_vbo.bind(1);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(0 * sizeof(glm::vec4)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(1 * sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    //
    boids_vbo.unbind();
    vao.unbind();

    Vao cube_vao(1);
    cube_vao.gen();
    cube_vao.bind();
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    cube_vbo.bind(0);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, uv));
    cube_vbo.unbind();
    cube_vbo.unbind();
    cube_vao.unbind();

    Flock flock = Flock();
    for(int i = 0 ; i < params._boids_number ; i++)
    {
        Boid tmp;
        flock.add_boid(tmp);
    }
    Force avoidance(params._multiplicator_avoidance); //Tendance à augmenter la distance inter-boids
    Force alignement(params._multiplicator_alignement); //Tendance à former des gros groupes facilement
    Force centering(params._multiplicator_centering); //Tendance à diminuer le rayon d'un groupe de boid

    globalMatrix gm;
    
    uGlobalMatrix ugm;
    getUniformLocations(true, shader, ugm);

    uGlobalMatrix cube_ugm;
    getUniformLocations(false, draw_shader, cube_ugm);

    texture boids_texture("../textures/shark_texture.png");
    texture cube_texture("../textures/cube_texture.png");
    GLint uTexture = glGetUniformLocation(shader.id(), "uTexture");

    light point_1(glm::vec3(0,0,0), glm::vec3(150,150,150));
    light_uniforms l_uniforms;
    material_params mat_params(glm::vec3(0,1,0), glm::vec3(1,1,1), 1);

    get_uniforms(shader, l_uniforms);
    // GLint uKd = glGetUniformLocation(shader.id(), "uKd");
    // GLint uKs = glGetUniformLocation(shader.id(), "uKs");
    // GLint uShininess= glGetUniformLocation(shader.id(), "uShininess");
    // GLint uLightPos_vs= glGetUniformLocation(shader.id(), "uLightPos_vs");
    // GLint uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::Color(0.2,0.4,0.6));
        ctx.square(p6::Center{}, p6::Radius{1.f});
        gm.ProjMatrix = glm::perspective(glm::radians(110.f), ctx.aspect_ratio(), 0.1f, 100.f);

        glEnable(GL_CULL_FACE); //Hide the back faces of the model
        glEnable(GL_DEPTH_TEST); //Checks if the fragment has to be rendered based on it's z value

        //Drawing the cube
        draw_shader.use();
        matricesCube(gm, cube_ugm);
        glBindTexture(GL_TEXTURE_2D, cube_texture.texture_id); 
        glUniform1i(uTexture, 0);

        point_1.set_intensity(glm::vec3(150,150,150));
        set_uniforms(l_uniforms, mat_params, point_1);

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
            //instanc_matrix[i] = glm::rotate(instanc_matrix[i], glm::degrees(glm::acos(glm::dot(e[i].get_direction(), e[i].get_velocity()))), glm::cross(e[i].get_direction(), e[i].get_velocity()));
        }

        boids_vbo.bind(1);
        glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instanc_matrix.data(), GL_DYNAMIC_READ); 
        boids_vbo.unbind();

        shader.use();
        
        for(size_t i = 0; i<params._boids_number; i++)
        {
            matricesBoids(gm, ugm, instanc_matrix[i]);
        }
        glBindTexture(GL_TEXTURE_2D, boids_texture.texture_id);
        glUniform1i(uTexture, 0);

        point_1.set_intensity(glm::vec3(1,1,1));
        set_uniforms(l_uniforms, mat_params, point_1);

        vao.bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, suzanne.vertices.size(), params._boids_number);
        glBindTexture(GL_TEXTURE_2D, 0);
        vao.unbind();
        glClear(GL_DEPTH_BUFFER_BIT);  
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    //normalement ici les boids_vbo et vao sont détruits automatiquement par les destructeurs désignés
}
