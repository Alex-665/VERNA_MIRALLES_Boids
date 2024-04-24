#include "glm/fwd.hpp"
#include "p6/p6.h"
#include "include_glm.hpp"

struct light_uniforms
{
    GLint m_uKd;
    GLint m_uKs;
    GLint m_uShininess;
    GLint m_uLightPos;
    GLint m_uLightIntensity;
};

struct material_params
{
    glm::vec3 m_uKd;
    glm::vec3 m_uKs;
    float m_uShininess;

    material_params(glm::vec3 uKd, glm::vec3 uKs, float uShininess): m_uKd(uKd), m_uKs(uKs), m_uShininess(uShininess){};
};

class light
{
    private:
        glm::vec3 m_position;
        glm::vec3 m_intensity;
    public:
        light(glm::vec3 pos, glm::vec3 intensity) : m_position(pos), m_intensity(intensity){};
        inline glm::vec3 get_position() const {return m_position;};
        inline glm::vec3 get_intensity() {return m_intensity;};
        inline void set_position(glm::vec3 pos) {m_position = pos;};
        inline void set_intensity(glm::vec3 intensity) {m_intensity = intensity;};
};

void get_uniforms(const p6::Shader &shader, light_uniforms &l_u);
void set_uniforms(const light_uniforms l_u, const material_params m_p, glm::vec4 light_positions[], glm::vec3 light_intensities[]);