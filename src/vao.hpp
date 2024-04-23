#pragma once
#include <vector>
#include "p6/p6.h"

class Vao {
    private:
        GLuint m_n {};
        std::vector<GLuint> m_buffers {};
    public:
        Vao(GLsizei n): m_n(n), m_buffers(std::vector<GLuint> (m_n)) {};
        Vao(Vao &v): m_n (v.get_n()), m_buffers(v.get_buffers()) {};
        ~Vao() {glDeleteVertexArrays(m_n, m_buffers.data());};
        inline void gen() {glGenVertexArrays(m_n, m_buffers.data());};
        inline void bind() const {glBindVertexArray(m_buffers[0]);};
        inline void bind(const int i) const {glBindVertexArray(m_buffers[i]);};
        inline void unbind() {glBindVertexArray(0);};
        inline GLuint get_n() {return m_n;};
        inline std::vector<GLuint> get_buffers() {return m_buffers;};
};
