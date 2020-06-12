#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
private:
    glm::vec3 cameraPos;    // Position of the camera in 3D world.
    glm::vec3 cameraFront;  // When added with cameraPos gives the camera target.
    glm::vec3 upVector;     // Used to calculate the "right vector" in LookAt function.
    
    float cameraSpeed;      // How fast the camera can move around the scene
    float sensitivity;      // How fast the camera can change direction.
    float zoomSensitivity;  // How fast the camera can zoom in/out of something.
    float boostFactor;

    double yaw;             // Euler anlge: Yaw
    double pitch;           // Euler angle: Pitch
public:
    float Fov;              // Field of view.
public: 
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,

        BOOST,
    };
public:
    Camera(const glm::vec3& cameraPos);
    void ProcessMouseMovement(double xOffset, double yOffset);
    void ProcessMouseScroll(double yOffset);
    void ProcessKeyboard(CameraMovement movement, double deltaTime);
    glm::mat4 GetViewMatrix()const;
    void SetCameraSpeed(float speed);
    void SetSensitivity(float sensitivity);
    void SetZoomSensitivity(float zoomSensitivity);
private:
    glm::mat4 LookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& upVector)const;
};

