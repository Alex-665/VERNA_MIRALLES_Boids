#pragma once
#include "vao.hpp"
#include "vbo.hpp"
#include "loader.h"
#include "light.hpp"
#include "matrix.hpp"
#include "texture.hpp"

class Renderer {
    private:
        Vao m_vao;
        Vbo m_vbo;
        Object3D m_object;
        const p6::Shader &m_shader;
        GlobalMatrix m_gm;
        uGlobalMatrix m_ugm;
        void setupBufferData();
        void setupAttribArray(const bool instanced);
    public:
        Renderer(Vao &vao, Vbo &vbo, Object3D &object, const p6::Shader &shader, const bool instanced): m_vao(vao), m_vbo(vbo), m_object(object), m_shader(shader)
    {
        setupBufferData();
        setupAttribArray(instanced);
    };
        void set_matrix(GlobalMatrix &gm, uGlobalMatrix &ugm);
        void draw_classic(Texture &texture, LightUniforms &l_uniforms, MaterialParams &mat_params, glm::vec4 light_positions[], glm::vec3 light_intensities[]);
        void drawInstanced(Texture &texture, LightUniforms &l_uniforms, MaterialParams &mat_params, glm::vec4 light_positions[], glm::vec3 light_intensities[], int number);
};
