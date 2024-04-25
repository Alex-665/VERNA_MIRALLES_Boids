#pragma once
#include "p6/p6.h"

class Texture
{
    public:
    GLuint texture_id;
    GLint u_texture;
    Texture(const char* path);
};
