#include "renderer.hpp"

void Renderer::drawClassic()
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
