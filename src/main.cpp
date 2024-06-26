#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include "glm/trigonometric.hpp"
#include "probas.hpp"
#include <iostream>
#include "vao.hpp"
#include "vbo.hpp"
#include "matrix.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "light.hpp"
#include "renderer.hpp"
#include "freeflyCamera.hpp"
#include "GLFW/glfw3.h"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code

    Parameters params;
    static int LOD = 0;
    static std::string etat_string = "En forme";

    auto ctx = p6::Context{{.width = 720, .height = 720, .title="Simple-p6-Setup"}};
    ctx.imgui = [&]() {
        ImGui::Begin("Caractéristiques");
        ImGui::SliderFloat("avoidance multiplicator", &params._multiplicator_avoidance, 0.1f, 50.f);
        ImGui::SliderFloat("centering multiplicator", &params._multiplicator_centering, 0.1f, 20.f);
        ImGui::SliderFloat("alignement multiplicator", &params._multiplicator_alignement, 0.1f, 20.f);
        ImGui::SliderInt("boids number", &params._boids_number, 1, 1000);
        ImGui::CheckboxFlags("High Details", &LOD, 1);
        ImGui::Text("Etat : %s", etat_string.c_str());
        ImGui::End();
    };

    srand(static_cast<unsigned int>(time(NULL))); // Initialize random seed

    //Definitions of useful variables
    const p6::Shader instanced_shader(std::string("#version 330 core\n") + std::string("#define INSTANCING\n") + file_content("src/shaders/red.vs.glsl"), file_content("src/shaders/point_light.fs.glsl"));

    const p6::Shader classic_shader(std::string("#version 330 core\n") + file_content("src/shaders/red.vs.glsl"), file_content("src/shaders/point_light.fs.glsl"));
    
    MarkovChain etat(glm::vec3(1,0,0), glm::mat3(glm::vec3(0.75, 0.20, 0.05), glm::vec3(0.25, 0.5, 0.25), glm::vec3(0.05, 0.35, 0.6)));

    FreeflyCamera camera;

    Object3D rabbit = load_obj("../models/rabbit.obj");
    Vbo boids_vbo(2);
    boids_vbo.gen();
    Vao vao(1);
    vao.gen();
    Renderer boids_renderer(vao, boids_vbo, rabbit, instanced_shader, true);

    Object3D cube = load_obj("../models/cube.obj");
    Vbo cube_vbo(1);
    cube_vbo.gen();
    Vao cube_vao(1);
    cube_vao.gen();
    Renderer cube_renderer(cube_vao, cube_vbo, cube, classic_shader, false);

    Object3D rabbit_high = load_obj("../models/rabbit_high.obj");
    Vbo boids_highpoly_vbo(2);
    boids_highpoly_vbo.gen();
    Vao vao_highpoly(1);
    vao_highpoly.gen();
    Renderer boids_highpoly_renderer(vao_highpoly, boids_highpoly_vbo, rabbit_high, instanced_shader, true);

    Object3D swan = load_obj("../models/swan.obj");
    Vbo swan_vbo(1);
    swan_vbo.gen();
    Vao swan_vao(1);
    swan_vao.gen();
    Renderer swan_renderer(swan_vao, swan_vbo, swan, classic_shader, false);

    Flock flock(params._boids_number);

    Force avoidance(params._multiplicator_avoidance); //Tendance à augmenter la distance inter-boids
    Force alignement(params._multiplicator_alignement); //Tendance à former des gros groupes facilement
    Force centering(params._multiplicator_centering); //Tendance à diminuer le rayon d'un groupe de boid

    GlobalMatrix gm;
    
    uGlobalMatrix boids_ugm;
    get_uniform_locations(true, instanced_shader, boids_ugm);
    uGlobalMatrix cube_ugm;
    get_uniform_locations(false, classic_shader, cube_ugm);
    uGlobalMatrix swan_ugm;
    get_uniform_locations(false, classic_shader, swan_ugm);

    Texture boids_texture("../textures/Rabbit_texture.png");
    Texture cube_texture("../textures/cube_texture.png");
    Texture swan_texture("../textures/swan_texture.png");
    GLint uTexture = glGetUniformLocation(instanced_shader.id(), "uTexture");
    boids_texture.u_texture = uTexture; 
    cube_texture.u_texture = uTexture; 
    swan_texture.u_texture = uTexture; 

    Light point_1(glm::vec3(0,0,0), glm::vec3(200, 150, 150));
    Light point_2(glm::vec3(0,0,0), glm::vec3(150,100,50));

    glm::vec4 light_positions[] = {
        glm::vec4(point_1.get_position(),1),
        glm::vec4(point_2.get_position(),1)
    };
    glm::vec3 light_intensities[] = {
        point_1.get_intensity(),
        point_2.get_intensity()
    };

    LightUniforms l_uniforms;
    MaterialParams mat_params(glm::vec3(1,1,1), glm::vec3(1,1,1), 1);

    Arpenteur player;
    float birth_time{2.f};
    float death_time{2.f};

    ctx.update = [&]() {
        ctx.background(p6::Color(0.2,0.4,0.6));
        ctx.square(p6::Center{}, p6::Radius{1.f});
        gm.proj_matrix = glm::perspective(glm::radians(56.f), ctx.aspect_ratio(), 0.1f, 200.f);

        player.move_third_person(ctx, camera);

        gm.view_matrix = camera.get_view_matrix(player.get_position());
        point_1.set_position(player.get_position());
        light_positions[0] = gm.view_matrix * glm::vec4(point_1.get_position(), 1);
        light_positions[1] = gm.view_matrix * glm::vec4(point_2.get_position(), 1);
        
        glEnable(GL_CULL_FACE); //Hide the back faces of the model
        glEnable(GL_DEPTH_TEST); //Checks if the fragment has to be rendered based on it's z value

        //Draw the cube and swan
        classic_shader.use();

        matrices_cube(gm, cube_ugm);
        cube_renderer.set_matrix(gm, cube_ugm);
        cube_renderer.draw_classic(cube_texture, l_uniforms, mat_params, light_positions, light_intensities);

        matrices_swan(gm, swan_ugm, player);
        swan_renderer.set_matrix(gm, swan_ugm);
        swan_renderer.draw_classic(swan_texture, l_uniforms, mat_params, light_positions, light_intensities);

        flock.update(ctx.delta_time(), 1, params);
        
        std::vector<glm::mat4> instance_mv_matrix(params._boids_number);
        std::vector<Boid> e = flock.get_boids();  
        for(size_t i = 0; i<params._boids_number; i++) {
            instance_mv_matrix[i] = translate(e[i].get_position().x, e[i].get_position().y, e[i].get_position().z);
            instance_mv_matrix[i] =  instance_mv_matrix[i] * scale(1, 1, 1);
            instance_mv_matrix[i] = glm::rotate(instance_mv_matrix[i], glm::acos(glm::dot(glm::normalize(e[i].get_direction()), glm::normalize(e[i].get_velocity()))), glm::cross(glm::normalize(e[i].get_direction()), glm::normalize(e[i].get_velocity())));
            instance_mv_matrix[i] = gm.view_matrix * instance_mv_matrix[i];
        }

        if(LOD == 0){
            boids_vbo.bind(1);
            glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instance_mv_matrix.data(), GL_DYNAMIC_READ); 
            boids_vbo.unbind();
        }
        else {
            boids_highpoly_vbo.bind(1);
            glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instance_mv_matrix.data(), GL_DYNAMIC_READ); 
            boids_highpoly_vbo.unbind();
        }

        //Draw the boids
        instanced_shader.use();
        
        for(size_t i = 0; i<params._boids_number; i++)
        {
            matrices_boids(gm, boids_ugm, instance_mv_matrix[i]);
        }

        if(LOD == 0) {
            boids_renderer.set_matrix(gm, boids_ugm);
            boids_renderer.drawInstanced(boids_texture, l_uniforms, mat_params, light_positions, light_intensities, params._boids_number);
        }
        else {
            boids_highpoly_renderer.set_matrix(gm, boids_ugm);
            boids_highpoly_renderer.drawInstanced(boids_texture, l_uniforms, mat_params, light_positions, light_intensities, params._boids_number);
        }

        glClear(GL_DEPTH_BUFFER_BIT);  

        player.shape_state(ctx, etat, etat_string);
        computeEvolution(flock, ctx, birth_time, death_time, params._boids_number);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
