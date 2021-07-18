#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <array>
#include <chrono>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "shaderProgram.h"
#include "texture.h"

void processUserInput(GLFWwindow* window);

std::array<float, 180> vertices{
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

glm::vec3 cameraPosition{ 0.0f, 0.0f, 0.0f };
glm::vec3 cameraFront { 0.0f, 0.0f, 1.0f };
glm::vec3 cameraRight { 0.0f, 0.0f, 0.0f };
glm::vec3 cameraUp { 0.0f, 1.0f, 0.0f };

std::chrono::high_resolution_clock::time_point currentTime;
std::chrono::milliseconds deltaFrameTime;

struct MVP
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};


int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);   
    if(!window) {
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);

    auto vertShader = Shader(GL_VERTEX_SHADER);
    vertShader.compileShader("shaders/shader.vert");

    auto fragShader = Shader(GL_FRAGMENT_SHADER);
    fragShader.compileShader("shaders/shader.frag");

    ShaderProgram program;
    program.attachShader(vertShader);
    program.attachShader(fragShader);
    program.linkProgram();
    vertShader.deleteShader();
    fragShader.deleteShader();

    Texture texture("textures/container.jpg");
    cameraPosition = { 0.0f, 0.0f, -4.0f };
    cameraRight = glm::normalize(glm::cross(cameraUp, cameraPosition + cameraFront));

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    MVP mvp { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

    mvp.projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

    currentTime = std::chrono::high_resolution_clock::now();

    while(!glfwWindowShouldClose(window)) 
    {
        deltaFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime);
        currentTime = std::chrono::high_resolution_clock::now();

        processUserInput(window);

        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mvp.model = glm::rotate(mvp.model, glm::radians(0.05f * deltaFrameTime.count()), glm::vec3(0.0f, 1.0f, 1.0f));
        mvp.view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
 
        program.setMat4("model", mvp.model);
        program.setMat4("projection", mvp.projection);
        program.setMat4("view", mvp.view);

        glBindTexture(GL_TEXTURE_2D, texture.getId());
        program.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    program.deleteProgram();
    glfwTerminate();

    return 0;
}

void processUserInput(GLFWwindow* window)
{
    float speed = 0.01f * deltaFrameTime.count();

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += speed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= speed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {        
        cameraPosition += speed * cameraRight;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= speed * cameraRight;
    }
    // TODO : make camera rotation around cameraUp axis by Q and E keys
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}