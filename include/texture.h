#pragma once

#include <glad/glad.h>
#include <stb_image.h>

class Texture
{
public:
    Texture(const char* filename);
    void loadImage(const char* filename);
    unsigned int getId();
    ~Texture();
private:
    int width, height, channels;
    unsigned char* data;
    unsigned int texture;
};