#pragma once
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include "include_glm.hpp"

struct LightUniforms
{
    GLint m_uKd;
    GLint m_uKs;
    GLint m_uShininess;
    GLint m_uLightPos;
    GLint m_uLightIntensity;
};

struct MaterialParams
{
    glm::vec3 m_uKd;
    glm::vec3 m_uKs;
    float m_uShininess;

    MaterialParams(glm::vec3 uKd, glm::vec3 uKs, float uShininess): m_uKd(uKd), m_uKs(uKs), m_uShininess(uShininess){};
};

class Light
{
    private:
        glm::vec3 m_position;
        glm::vec3 m_intensity;
    public:
        Light(glm::vec3 pos, glm::vec3 intensity) : m_position(pos), m_intensity(intensity){};
        inline glm::vec3 get_position() const {return m_position;};
        inline glm::vec3 get_intensity() {return m_intensity;};
        inline void set_position(glm::vec3 pos) {m_position = pos;};
        inline void set_intensity(glm::vec3 intensity) {m_intensity = intensity;};
};

void get_uniforms(const p6::Shader &shader, LightUniforms &l_u);
void set_uniforms(const LightUniforms l_u, const MaterialParams m_p, glm::vec4 light_positions[], glm::vec3 light_intensities[]);
