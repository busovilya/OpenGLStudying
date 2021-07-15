#include "texture.h"

#include <stdexcept>
#include <string>

Texture::Texture(const char* filename)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadImage(filename);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
 
    stbi_image_free(data);
}

void Texture::loadImage(const char* filename)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename, &width, &height, &channels, 0);

    if(!data)
    {
        throw std::runtime_error("Failed to load texture from " + std::string(filename));
    }
}

unsigned int Texture::getId()
{
    return texture;
}

Texture::~Texture()
{
}