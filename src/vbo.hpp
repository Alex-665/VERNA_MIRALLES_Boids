#pragma once

class Vbo {
    private:
        GLuint m_id {};
    public:
        Vbo() = default;
        ~Vbo() {glDeleteBuffers(1, &m_id);};
        inline void gen() {glGenBuffers(1, &m_id);};
        inline void bind() {glBindBuffer(GL_ARRAY_BUFFER, m_id);};
        inline void unbind() {glBindBuffer(GL_ARRAY_BUFFER, 0);};
};
