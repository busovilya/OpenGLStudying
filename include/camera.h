#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(const glm::vec3& initialPosition, const glm::vec3& initialUp, const glm::vec3& initialRight);
    void move(const glm::vec3& offset);
    // void rotate(float angle, const glm::vec3& axis);
    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();
    glm::vec3 getUpVector();
    glm::vec3 getForwardVector();
    glm::vec3 getRightVector();
private:
    glm::vec3 position, cameraUp, cameraForward, cameraRight;
    glm::mat4 viewMatrix;
};