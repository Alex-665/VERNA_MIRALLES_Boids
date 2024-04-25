#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "probas.hpp"
#include "flock.hpp"
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
#include "renderer.hpp"
#include "freeflyCamera.hpp"
#include "arpenteur.hpp"
#include "GLFW/glfw3.h"

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code

    parameters params;
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

    const p6::Shader shader(std::string("#version 330 core\n") + std::string("#define INSTANCING\n") + file_content("src/shaders/red.vs.glsl"), file_content("src/shaders/point_light.fs.glsl"));

    const p6::Shader draw_shader(std::string("#version 330 core\n") + file_content("src/shaders/red.vs.glsl"), file_content("src/shaders/point_light.fs.glsl"));
    
    // HERE IS THE INITIALIZATION CODE
    MarkovChain etat(glm::vec3(1,0,0), glm::mat3(glm::vec3(0.75, 0.20, 0.05), glm::vec3(0.25, 0.5, 0.25), glm::vec3(0.05, 0.35, 0.6)));

    FreeflyCamera camera;

    Object3D rabbit = loadOBJ("../models/rabbit.obj");
    Vbo boids_vbo(2);
    boids_vbo.gen();
    Vao vao(1);
    vao.gen();
    Renderer boids_renderer(vao, boids_vbo, rabbit, true);

    Object3D cube = loadOBJ("../models/cube.obj");
    Vbo cube_vbo(1);
    cube_vbo.gen();
    Vao cube_vao(1);
    cube_vao.gen();
    Renderer cube_renderer(cube_vao, cube_vbo, cube, false);

    Object3D rabbit_high = loadOBJ("../models/rabbit_high.obj");
    Vbo boids_highpoly_vbo(2);
    boids_highpoly_vbo.gen();
    Vao vao_highpoly(1);
    vao_highpoly.gen();
    Renderer boids_highpoly_renderer(vao_highpoly, boids_highpoly_vbo, rabbit_high, true);

    Object3D swan = loadOBJ("../models/swan.obj");
    Vbo swan_vbo(1);
    swan_vbo.gen();
    Vao swan_vao(1);
    swan_vao.gen();
    Renderer swan_renderer(swan_vao, swan_vbo, swan, false);

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

    uGlobalMatrix swan_ugm;
    getUniformLocations(false, draw_shader, swan_ugm);

    texture boids_texture("../textures/Rabbit_texture.png");
    texture cube_texture("../textures/cube_texture.png");
    texture swan_texture("../textures/swan_texture.png");
    GLint uTexture = glGetUniformLocation(shader.id(), "uTexture");

    light point_1(glm::vec3(0,0,0), glm::vec3(500, 500, 500));
    light point_2(glm::vec3(0,0,0), glm::vec3(50,35,10));

    glm::vec4 light_positions[] = {
        glm::vec4(point_1.get_position(),1),
        glm::vec4(point_2.get_position(),1)
    };
    glm::vec3 light_intensities[] = {
        point_1.get_intensity(),
        point_2.get_intensity()
    };

    light_uniforms l_uniforms;
    material_params mat_params(glm::vec3(1,1,1), glm::vec3(1,1,1), 1);

    Arpenteur player;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::Color(0.2,0.4,0.6));
        ctx.square(p6::Center{}, p6::Radius{1.f});
        gm.ProjMatrix = glm::perspective(glm::radians(56.f), ctx.aspect_ratio(), 0.1f, 200.f);

        if (ctx.key_is_pressed(GLFW_KEY_LEFT)) {
            //camera.moveLeft(1.f);
            player.move_right(-ctx.delta_time());
        }
        if (ctx.key_is_pressed(GLFW_KEY_RIGHT)) {
            //camera.moveLeft(-1.f);
            player.move_right(ctx.delta_time());
        }
        if (ctx.key_is_pressed(GLFW_KEY_UP)) {
            //camera.moveFront(1.f);
            player.move_front(-ctx.delta_time());
        }
        if (ctx.key_is_pressed(GLFW_KEY_DOWN)) {
            //camera.moveFront(-1.f);
            player.move_front(ctx.delta_time());
        }
        if (ctx.key_is_pressed(GLFW_KEY_U)) {
            //camera.moveFront(-1.f);
            player.move_up(ctx.delta_time());
        }
        if (ctx.key_is_pressed(GLFW_KEY_D)) {
            //camera.moveFront(-1.f);
            player.move_up(-ctx.delta_time());
        }
        if (ctx.mouse_button_is_pressed(p6::Button::Left)) {
            camera.rotateLeft(-50.f * ctx.mouse_delta().x);
            camera.rotateUp(-50.f * ctx.mouse_delta().y);
        }
        gm.ViewMatrix = camera.getViewMatrix(player.get_position());
        point_1.set_position(player.get_position());
        light_positions[0] = gm.ViewMatrix * glm::vec4(point_1.get_position(), 1);
        
        glEnable(GL_CULL_FACE); //Hide the back faces of the model
        glEnable(GL_DEPTH_TEST); //Checks if the fragment has to be rendered based on it's z value

        //pour les bouts de code comme ça peut être faire en dehors de la classe des fonctions pour chaque cas particulier car c'est un peu chiant sinon
        //Drawing the cube
        draw_shader.use();
        matricesCube(gm, cube_ugm);
        glBindTexture(GL_TEXTURE_2D, cube_texture.texture_id); 
        glUniform1i(uTexture, 0);

        get_uniforms(draw_shader, l_uniforms);
        set_uniforms(l_uniforms, mat_params, light_positions, light_intensities);

        cube_renderer.drawClassic();

        //draw the swan
        matricesSwan(gm, swan_ugm, player);
        glBindTexture(GL_TEXTURE_2D, swan_texture.texture_id);

        get_uniforms(draw_shader, l_uniforms);
        set_uniforms(l_uniforms, mat_params, light_positions, light_intensities);

        swan_renderer.drawClassic();

        flock.update(ctx.delta_time(), 1, params);
        
        std::vector<glm::mat4> instanc_matrix(params._boids_number);
        std::vector<Boid> e = flock.get_boids();  
        for(size_t i = 0; i<params._boids_number; i++) {
            instanc_matrix[i] = translate(e[i].get_position().x, e[i].get_position().y, e[i].get_position().z);
            instanc_matrix[i] =  instanc_matrix[i] * scale(1, 1, 1);
            instanc_matrix[i] = glm::rotate(instanc_matrix[i], glm::acos(glm::dot(glm::normalize(e[i].get_direction()), glm::normalize(e[i].get_velocity()))), glm::cross(glm::normalize(e[i].get_direction()), glm::normalize(e[i].get_velocity())));
        }

        if(LOD == 0){
            boids_vbo.bind(1);
            glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instanc_matrix.data(), GL_DYNAMIC_READ); 
            boids_vbo.unbind();
        }
        else {
            boids_highpoly_vbo.bind(1);
            glBufferData(GL_ARRAY_BUFFER, params._boids_number * sizeof(glm::mat4), instanc_matrix.data(), GL_DYNAMIC_READ); 
            boids_highpoly_vbo.unbind();
        }

        //draw boids
        shader.use();
        
        for(size_t i = 0; i<params._boids_number; i++)
        {
            matricesBoids(gm, ugm, instanc_matrix[i]);
        }
        glBindTexture(GL_TEXTURE_2D, boids_texture.texture_id);
        glUniform1i(uTexture, 0);

        get_uniforms(shader, l_uniforms);
        set_uniforms(l_uniforms, mat_params, light_positions, light_intensities);

        if(LOD == 0) boids_renderer.drawInstanced(params._boids_number);
        else boids_highpoly_renderer.drawInstanced(params._boids_number);

        glClear(GL_DEPTH_BUFFER_BIT);  

        if(((int)(ctx.time()) % 5) == 0 && ((int)(ctx.time() -ctx.delta_time()) % 5) != 0) //On ne change d'état que toutes les 5 secondes
        {
            double random_uniform = rand01();
            if (random_uniform < etat.getState().x) 
            {   
                etat_string = "En Forme";
                player.set_speed(20.f);
            }
            else if (random_uniform < etat.getState().x + etat.getState().y) 
            {
                etat_string = "Fatigue Legere";
                player.set_speed(10.f);
            }
            else
            {
                etat_string = "Fatigue Lourde";
                player.set_speed(5.f);
            }
            etat.nextState();
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    //normalement ici les boids_vbo et vao sont détruits automatiquement par les destructeurs désignés
}
