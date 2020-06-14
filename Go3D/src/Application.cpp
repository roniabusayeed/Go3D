#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "DebugUtils.h"
#include "Renderer.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#ifdef _DEBUG 
#define TEXTURE_PATH "res/textures/battlefield.jpg"
#define SHADER_PATH "res/shaders/source.shader"
#else
#define TEXTURE_PATH "battlefield.jpg"
#define SHADER_PATH "source.shader"
#endif


void ProcessInput(GLFWwindow* window);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

static Shader* objectShader = nullptr;
static Shader* lightSourceShader = nullptr;

static glm::vec3 lightSourcePos = glm::vec3(1.2f, 1.0f, 1.5f);
static glm::vec3 initialCameraPos = glm::vec3(0.0f, 0.0f, 5.0f);

static Camera* camera = new Camera(initialCameraPos);

Renderer renderer;

// Delta time.
static float deltaTime = 0.f;
static void updateDeltaTime();


int main()
{
    // Initialize GLFW library.
    if (!glfwInit())
        return -1;

    // Configure GLFW.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  
    // Create window and OpenGL Context.
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Go3D", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Hide the cursor and capture it.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set callbacks.
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    
    // Make context current.
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize glad.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    // Vertex data.
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Generate buffers.
    VertexBuffer* vb = new VertexBuffer(vertices, sizeof(float) * 6 * 36);
    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push<float>(3);
    layout->Push<float>(3);

    VertexArray* objectVA = new VertexArray(*vb, *layout);
    VertexArray* lightSourceVA = new VertexArray(*vb, *layout);

    // Shaders
    objectShader = new Shader("res/shaders/object.shader");
    lightSourceShader = new Shader("res/shaders/lightSource.shader");

    // Set up the object.
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectShader->Bind();
    objectShader->SetUniform("u_model", objectModel);

    


    // Initial projection matrix.
    glm::mat4 projection = glm::perspective(glm::radians(camera->Fov),
        float(SCR_WIDTH) / (float)SCR_HEIGHT, 0.1f, 100.0f);
    objectShader->Bind();
    objectShader->SetUniform("u_projection", projection);
    lightSourceShader->Bind();
    lightSourceShader->SetUniform("u_projection", projection);

    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Render loop.
    while (!glfwWindowShouldClose(window))
    {
        updateDeltaTime();
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Move light source around over time.
        float theta = glm::radians((float)glfwGetTime()) * 100;
        const float radius = 2.0f;
        lightSourcePos = glm::vec3(radius * cos(theta), 0.0f, radius * sin(theta));

        glm::mat4 lightSourceModel = glm::mat4(1.0f);
        lightSourceModel = glm::translate(lightSourceModel, glm::vec3(lightSourcePos));
        lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.2f));
        lightSourceShader->Bind();
        lightSourceShader->SetUniform("u_model", lightSourceModel);

        // Send light source position to the objectShader for calculating diffuse lighting.
        objectShader->Bind();
        objectShader->SetUniform("u_lightSourcePosition", lightSourcePos);

        
        // Update view/camera each frame.
        glm::mat4 view = camera->GetViewMatrix();
        objectShader->Bind();
        objectShader->SetUniform("u_view", view);
        lightSourceShader->Bind();
        lightSourceShader->SetUniform("u_view", view);

        // Set the colors to the object.
        objectShader->Bind();
        objectShader->SetUniform("u_objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        objectShader->SetUniform("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        // Send camera position to object shader to calculate specular intensity.
        objectShader->SetUniform("u_cameraPosition", camera->GetPosition());

        // Render the object.
        renderer.Draw(*objectVA, *objectShader);
        renderer.Draw(*lightSourceVA, *lightSourceShader);

        // Swap buffers and poll input events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up.
    // TODO: Clean up all heap allocated objects.
    glfwTerminate();
}

void ProcessInput(GLFWwindow* window)
{
    // Close the application using escape key.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera around using keyboard.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraMovement::RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraMovement::BOOST, deltaTime);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Adjust viewport to frame buffer size.
    glViewport(0, 0, width, height);
    
    // Adjust projection matrix's aspect ratio, since it's (most probably)
    // changed as this function is called by glfw.
    glm::mat4 projection = glm::perspective(glm::radians(camera->Fov), (float)width / (float)height, 0.1f, 100.f);
    objectShader->Bind();
    objectShader->SetUniform("u_projection", projection);
    lightSourceShader->Bind();
    lightSourceShader->SetUniform("u_projection", projection);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Get x and y offsets of mouse movement since last time these offsets were calculated.
    static bool firstUse = true;
    static double lastX;
    static double lastY;
    if (firstUse)
    {
        lastX = xPos;
        lastY = yPos;
        firstUse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;   // Don't look at me. You are weird too!
    lastX = xPos;
    lastY = yPos;

    // Recalculate camera direction using these offsets.
    camera->ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    // Update camera Fov.
    camera->ProcessMouseScroll(yOffset);

    // Calculate aspect ratio.
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    glm::mat4 projection = glm::perspective(glm::radians(camera->Fov),
        (float)width/(float)height, 0.1f, 100.0f);
    objectShader->Bind();
    objectShader->SetUniform("u_projection", projection);
    lightSourceShader->Bind();
    lightSourceShader->SetUniform("u_projection", projection);
}

void updateDeltaTime()
{
    static float lastFrameTime = 0.0f;
    float currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
}
