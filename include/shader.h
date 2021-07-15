#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "glad/glad.h"

class Shader
{
public:
    Shader(GLenum shaderType);
    void compileShader(std::string shaderFileName);
    GLenum getShaderType() const;
    int getShaderId() const;
    std::string getTypeName();
    void deleteShader();
private:
    int shader;
    GLenum shaderType;
};