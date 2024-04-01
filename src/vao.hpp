#pragma once

class Vao {
    private:
        GLuint m_id {};
    public:
        Vao() = default;
        ~Vao() {glDeleteVertexArrays(1, &m_id);};
        inline void gen() {glGenVertexArrays(1, &m_id);};
        inline void bind() {glBindVertexArray(m_id);};
        inline void unbind() {glBindVertexArray(0);};
};
