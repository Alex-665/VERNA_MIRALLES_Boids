#include "texture.hpp"
#include <memory>
#include "img/src/Image.h"

texture::texture(const char* path)
{
    auto texture_ptr = p6::load_image_buffer(path);
    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_ptr.width(), texture_ptr.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ptr.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}