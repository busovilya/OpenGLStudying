#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"

class ShaderProgram
{
public:
    ShaderProgram();
    void attachShader(const Shader& shader);
    void linkProgram();
    void deleteProgram();
    int getProgramId();
    void setMat4(std::string name, const glm::mat4& matrix);
    void use();
private:
    int program;
};