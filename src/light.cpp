#include "light.hpp"
#include <vector>

void get_uniforms(const p6::Shader &shader, light_uniforms &l_u)
{
    l_u.m_uKd = glGetUniformLocation(shader.id(), "uKd");
    l_u.m_uKs = glGetUniformLocation(shader.id(), "uKs");
    l_u.m_uShininess= glGetUniformLocation(shader.id(), "uShininess");
    l_u.m_uLightPos= glGetUniformLocation(shader.id(), "uLightPos");
    l_u.m_uLightIntensity = glGetUniformLocation(shader.id(), "uLightIntensity");
}

void set_uniforms(const light_uniforms l_u, const material_params m_p, glm::vec3 light_positions[], glm::vec3 light_intensities[])
{
    glUniform3fv(l_u.m_uKd, 1, glm::value_ptr(m_p.m_uKd)); //Uniform values for lighting
    glUniform3fv(l_u.m_uKs, 1, glm::value_ptr(m_p.m_uKs));
    glUniform1f(l_u.m_uShininess, m_p.m_uShininess);
    glUniform3fv(l_u.m_uLightPos, light_positions->length(), glm::value_ptr(light_positions[0]));
    glUniform3fv(l_u.m_uLightIntensity, light_intensities->length(), glm::value_ptr(light_intensities[0]));
}