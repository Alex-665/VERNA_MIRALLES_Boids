#pragma once
#include <vector>

class Vbo {
    private:
        GLsizei m_n {};
        std::vector<GLuint> m_buffers {};
    public:
        Vbo(GLsizei n): m_n(n), m_buffers(std::vector<GLuint> (m_n)) {};
        ~Vbo() {glDeleteBuffers(m_n, m_buffers.data());};
        inline void gen() {glGenBuffers(m_n, m_buffers.data());};
        inline void bind() const {glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);};
        inline void bind(const int i) const {glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);};
        inline void unbind() const {glBindBuffer(GL_ARRAY_BUFFER, 0);};
};
