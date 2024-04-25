#pragma once
#include "vao.hpp"
#include "vbo.hpp"
#include "loader.h"

class Renderer {
    private:
        Vao m_vao;
        Vbo m_vbo;
        Object3D m_object;
        //p6::Shader m_shader;
        void setupBufferData();
        void setupAttribArray(const bool instanced);
    public:
        Renderer(Vao &vao, Vbo &vbo, Object3D &object, const bool instanced): m_vao(vao), m_vbo(vbo), m_object(object)
    {
        setupBufferData();
        setupAttribArray(instanced);
    };
        void draw_classic();
        void drawInstanced(int number);
};
