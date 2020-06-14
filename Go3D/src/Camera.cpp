#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>

Camera::Camera(const glm::vec3& cameraPos)
    : cameraPos(cameraPos),
      cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
      upVector(glm::vec3(0.0f, 1.0f, 0.0f))
{
    cameraSpeed     =   2.5f;     // Defualt camera Speed.
    sensitivity     =   0.1f;     // Default camera sensitivity.
    zoomSensitivity =   0.5f;     // Default camera zoom sensitivity.
    boostFactor     =   2.0f;     // Defualt camera boost fractor.

    // Euler angles
    yaw = -90.f;                // yaw is initialized to -90 deg since a yaw of zero results
                                // in a movement verctor pointing to the right. So we initially
                                // rotate a bit to the left.
    pitch = 0.0f;

    Fov = 45.0;                 // Default value for FOV (Field of View).
}

void Camera::ProcessMouseMovement(double xOffset, double yOffset)
{
    // Update Euler angles.
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    // Constraints.
    if (pitch > 89)
        pitch = 89;
    if (pitch < -89)
        pitch = -89;

    // Calculate camera movement using Euler angles.
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    cameraFront = direction;
}

/***
 *  This method updates the Fov field of this Camera class.
 *  It does not modify the FOV (Field of View) in projection
 *  matrix. So, to create the zoom effect a new projection 
 *  matrix needs to be created using Fov field of this class
 *  as FOV (Field of View) of perspective projection matrix
 *  after converting it to radians.
 *  to radians.*/
void Camera::ProcessMouseScroll(double yOffset)
{
    Fov -= yOffset * zoomSensitivity;

    // Constraints.
    if (Fov > 45)
        Fov = 45;
    if (Fov < 1)
        Fov = 1;
}

void Camera::ProcessKeyboard(CameraMovement movement, double deltaTime)
{    
    if (movement == CameraMovement::FORWARD)
        cameraPos += cameraFront * cameraSpeed * (float)deltaTime;
    if (movement == CameraMovement::BACKWARD)
        cameraPos -= cameraFront * cameraSpeed * (float)deltaTime;
    if (movement == CameraMovement::LEFT)
        cameraPos -= glm::normalize(glm::cross(cameraFront, upVector)) * cameraSpeed * (float)deltaTime;
    if (movement == CameraMovement::RIGHT)
        cameraPos += glm::normalize(glm::cross(cameraFront, upVector)) * cameraSpeed * (float)deltaTime;

    static bool boosted = false;
    if (movement == CameraMovement::BOOST)
    {
        if (!boosted)
        {
            cameraSpeed *= boostFactor;
            boosted = true;
        }
    }
    else
    {
        if (boosted)
        {
            cameraSpeed /= boostFactor;
            boosted = false;
        }
    }

    // Since a true FPS camera cannot fly, restrict the camera on zx (ground) plane.
    // cameraPos.y = 0.0f;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return LookAt(cameraPos, cameraPos + cameraFront, upVector);
}

void Camera::SetCameraSpeed(float cameraSpeed)
{
    this->cameraSpeed = cameraSpeed;
}

void Camera::SetSensitivity(float sensitivity)
{
    this->sensitivity = sensitivity;
}

void Camera::SetZoomSensitivity(float zoomSensitivity)
{
    this->zoomSensitivity = zoomSensitivity;
}

glm::vec3 Camera::GetPosition() const
{
    return cameraPos;
}

glm::mat4 Camera::LookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& upVector) const
{
    // glm also has a look at function.
    // However, this is my implementation.

    glm::vec3 camZ = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 camX = glm::normalize(glm::cross(upVector, camZ));
    glm::vec3 camY = glm::cross(camZ, camX);

    glm::mat4 I = glm::mat4(1.0f);

    glm::mat4 translate = glm::translate(I, -cameraPos);
    glm::mat4 rotate = glm::transpose(glm::mat4(
        glm::vec4(camX, 0.f),
        glm::vec4(camY, 0.f),
        glm::vec4(camZ, 0.f),
        glm::vec4(glm::vec3(0.f, 0.f, 0.f), 1.f)
    ));

    return rotate * translate;
}
