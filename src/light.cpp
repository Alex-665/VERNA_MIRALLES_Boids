#include "light.hpp"

void get_uniforms(const p6::Shader &shader, light_uniforms &l_u)
{
    l_u.m_uKd = glGetUniformLocation(shader.id(), "uKd");
    l_u.m_uKs = glGetUniformLocation(shader.id(), "uKs");
    l_u.m_uShininess= glGetUniformLocation(shader.id(), "uShininess");
    l_u.m_uLightPos_vs= glGetUniformLocation(shader.id(), "uLightPos_vs");
    l_u.m_uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");
}

void set_uniforms(const light_uniforms l_u, const material_params m_p, light &l)
{
    glUniform3fv(l_u.m_uKd, 1, glm::value_ptr(m_p.m_uKd)); //Uniform values for lighting
    glUniform3fv(l_u.m_uKs, 1, glm::value_ptr(m_p.m_uKs));
    glUniform1f(l_u.m_uShininess, m_p.m_uShininess);
    glUniform3fv(l_u.m_uLightPos_vs, 1, glm::value_ptr(l.get_position()));
    glUniform3fv(l_u.m_uLightIntensity, 1, glm::value_ptr(l.get_intensity()));
}