#include "camera.h"

#include <iostream>

Camera::Camera() : position(0.0f), cameraUp(0.0f), cameraForward(0.0f), cameraRight(0.0f), viewMatrix(0.0f)
{ }

Camera::Camera(const glm::vec3& initialPosition, const glm::vec3& initialUp, const glm::vec3& initialForward) :
    position(initialPosition), cameraUp(glm::normalize(initialUp)), cameraForward(glm::normalize(initialForward))
{ 
    cameraRight = glm::normalize(glm::cross(cameraForward, cameraUp)); 
    viewMatrix = glm::lookAt(position, position + cameraForward, cameraUp);
}

void Camera::move(const glm::vec3& offset)
{
    if(offset != glm::vec3(0.0f)) 
    {
        position += offset;
        viewMatrix = glm::lookAt(position, position + cameraForward, cameraUp);
    }
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::getForwardVector()
{
    return cameraForward;
}

glm::vec3 Camera::getUpVector()
{
    return cameraUp;
}

glm::vec3 Camera::getRightVector()
{
    return cameraRight;
}

glm::mat4 Camera::getViewMatrix()
{
    return viewMatrix;
}