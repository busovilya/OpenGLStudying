#include "shader.h"

Shader::Shader(GLenum _shaderType) : shaderType(_shaderType)
{
    shader = glCreateShader(shaderType);
    if(shader == 0) 
    {
        throw std::runtime_error("Failed to create shader with specified type!");
    }
}

void Shader::compileShader(std::string shaderFilename)
{
    std::string shaderSrc;
    std::ifstream in(shaderFilename, std::ios::binary);
    if(!in.is_open()) {
        throw std::runtime_error("Failed to read shader code from file " + shaderFilename + "!");
    }

    std::stringstream inStream;
    inStream << in.rdbuf();
    shaderSrc = inStream.str();
    in.close();

    const char* shaderCode = shaderSrc.c_str();
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    char infoLog[512];
    if(!status)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::" + getTypeName() + "::COMPILATION_FAILED\n" + std::string(infoLog));
    }
}

GLenum Shader::getShaderType() const
{
    return shaderType;   
}

int Shader::getShaderId() const
{
    return shader;
}

std::string Shader::getTypeName()
{
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        return "VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT_SHADER"; 
    }

    return "UNKNOWN";
}

void Shader::deleteShader()
{
    glDeleteShader(shader);
}