#pragma once

class Vbo {
    private:
        GLsizei m_n {};
        GLuint *m_buffers {};
    public:
        Vbo() = default;
        Vbo(GLsizei n): m_n(n), m_buffers(new GLuint[n]) {};
        ~Vbo() {glDeleteBuffers(m_n, m_buffers);};
        inline void gen() const {glGenBuffers(m_n, m_buffers);};
        inline void bind() const {glBindBuffer(GL_ARRAY_BUFFER, *m_buffers);};
        inline void bind(const int i) const {glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);};
        inline void unbind() const {glBindBuffer(GL_ARRAY_BUFFER, 0);};
};
