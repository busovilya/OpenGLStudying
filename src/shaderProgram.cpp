#include "shaderProgram.h"

ShaderProgram::ShaderProgram()
{
    program = glCreateProgram();
    if(program == 0)
    {
        throw std::runtime_error("Failed to creat shader program!");
    }
}

void ShaderProgram::attachShader(const Shader& shader)
{
    glAttachShader(program, shader.getShaderId());
}

void ShaderProgram::linkProgram()
{
    glLinkProgram(program);
    
    int status;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
}

void ShaderProgram::deleteProgram()
{
    glDeleteProgram(program);
}

int ShaderProgram::getProgramId() 
{
    return program;
}

void ShaderProgram::setMat4(std::string name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}