#include "renderer.hpp"

void Renderer::setupBufferData() 
{
    m_vbo.bind(0);
    glBufferData(GL_ARRAY_BUFFER, m_object.vertices.size() * sizeof(vertex), m_object.vertices.data(), GL_STATIC_DRAW);
    m_vbo.unbind();
}

void Renderer::setupAttribArray(const bool instanced)
{
    m_vao.bind();
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    m_vbo.bind(0);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)offsetof(vertex, uv));
    m_vbo.unbind();

    if (instanced) {
        //to enable the four vec4 of the instance_matrix
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);

        //way for the m_vao to read the matrix of transformation
        m_vbo.bind(1);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(0 * sizeof(glm::vec4)));
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(1 * sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(3 * sizeof(glm::vec4)));
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        //
        m_vbo.unbind();
    }
    m_vao.unbind();
}

void Renderer::draw_classic()
{
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, m_object.vertices.size());
    glBindTexture(GL_TEXTURE_2D, 0);
    m_vao.unbind();
}

void Renderer::drawInstanced(int number)
{
    m_vao.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_object.vertices.size(), number);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_vao.unbind();
}
