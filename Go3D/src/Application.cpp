#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "vendor/stb_image.h"
#include "DebugUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

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

static Shader* shader = nullptr;
static Camera* camera = nullptr;

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
        // Position             // Texture coordinates
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     // 0
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     // 1
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     // 2
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     // 3
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     // 4
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     // 5
                                                
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     // 6
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     // 7
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     // 8
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     // 9
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     // 10
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     // 11
                                                
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     // 12
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     // 13
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     // 14
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     // 15
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     // 16
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     // 17
                                                
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     // 18
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     // 29
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     // 20
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     // 21
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     // 22
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     // 23
                                                
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     // 24 
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     // 25
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     // 26
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     // 27
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     // 28
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     // 29
                                                
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     // 30
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     // 31
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     // 32
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     // 33
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     // 34
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f      // 35
    };

    // Generate buffers.
    unsigned int vb, va;
    glGenBuffers(1, &vb);
    glGenVertexArrays(1, &va);

    // Copy vertex buffer data.
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, 36 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Pack vertex buffer and vertex buffer layout into vertex array object.
    glBindVertexArray(va);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Shader
    shader = new Shader(SHADER_PATH);
    shader->Bind();

    
    // Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imgWidth, imgHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureData = stbi_load(TEXTURE_PATH, &imgWidth, &imgHeight, &nrChannels, 0);
    ASSERT(textureData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);

    // Bind the texture to texture slot=0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Send the texture bounded to slot=0 to the u_texture uniform.
    shader->SetUniform("u_texture", 0); 



    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Camera.
    camera =  new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

    // Default projection matrix.
    glm::mat4 projection = glm::perspective(glm::radians(camera->Fov),
        float(SCR_WIDTH) / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetUniform("u_projection", projection);


    // Render loop.
    while (!glfwWindowShouldClose(window))
    {
        updateDeltaTime();
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render.
        glBindVertexArray(va);
        
        // Update view/camera each frame.
        glm::mat4 view = camera->GetViewMatrix();
        shader->SetUniform("u_view", view);

        for (int i = 0; i < 10; i++)
        {
            // Draw 10 cubes in different places in the world (Using different
            // model matrix for each one).
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = glm::radians((float)glfwGetTime()/5 * (i + 1) * 20.0f);
            model = glm::rotate(model, angle, glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f)));
            
            shader->SetUniform("u_model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers and poll input events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up.
    glDeleteBuffers(1, &vb);
    glDeleteVertexArrays(1, &va);
    glDeleteTextures(1, &texture);
    delete shader;
    delete camera;
    glfwTerminate();
}

void ProcessInput(GLFWwindow* window)
{
    // Close the application using escape key.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera around using keyboard.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraDirection::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraDirection::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera::CameraDirection::RIGHT, deltaTime);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Adjust viewport to frame buffer size.
    glViewport(0, 0, width, height);
    
    // Adjust projection matrix's aspect ratio, since it's (most probably)
    // changed as this function is called by glfw.
    glm::mat4 projection = glm::perspective(glm::radians(camera->Fov), (float)width / (float)height, 0.1f, 100.f);
    shader->SetUniform("u_projection", projection);
}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    // Get x and y offsets of mouse movement since last time these offsets were calculated.
    static bool firstUse = true;
    static double lastX = SCR_WIDTH / 2;
    static double lastY = SCR_HEIGHT / 2;
    if (firstUse)
    {
        lastX = xPos;
        lastY = yPos;
        firstUse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
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
    shader->SetUniform("u_projection", projection);
}

void updateDeltaTime()
{
    static float lastFrameTime = 0.0f;
    float currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
}
