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

std::array<float, 20> vertices{
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left 
};

std::array<uint32_t, 6> indices{
    0, 1, 3, 1, 2, 3
};

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

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

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    MVP mvp { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

    mvp.projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    mvp.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    mvp.model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    auto startTime = std::chrono::high_resolution_clock::now();

    while(!glfwWindowShouldClose(window)) 
    {
        auto frameRenderingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);
        startTime = std::chrono::high_resolution_clock::now();

        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        mvp.model = glm::rotate(mvp.model, glm::radians(0.05f * frameRenderingDuration.count()), glm::vec3(0.0f, 0.0f, 0.1f));
 
        program.setMat4("model", mvp.model);
        program.setMat4("projection", mvp.projection);
        program.setMat4("view", mvp.view);

        glBindTexture(GL_TEXTURE_2D, texture.getId());
        glUseProgram(program.getProgramId());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    program.deleteProgram();
    glfwTerminate();

    return 0;
}