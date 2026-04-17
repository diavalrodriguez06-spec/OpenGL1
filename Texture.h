#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

class Texture {
public:
    unsigned int  ID;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    void texUnit(Shader& shader, const char* uniform, unsigned int unit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif // TEXTURE_CLASS_H



