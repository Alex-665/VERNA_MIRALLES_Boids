#pragma once
#include "vao.hpp"
#include "vbo.hpp"
#include "loader.h"

//attention si on veut passer un shader dans les attributs ça fait quelque chose de bizarre (mais y en n'a pas besoin pour l'instant donc je mets juste ça là au cas où)
class Renderer {
    private:
        Vao m_vao;
        Object3D m_object;
    public:
        //note : passer le vao en référence parce que sinon c'est fait par copie et forcément opengl aime pas trop bcp ça
        Renderer(Vao &vao, Object3D &object): m_vao(vao), m_object(object) {};
        void drawClassic();
        void drawInstanced(int number);
};
