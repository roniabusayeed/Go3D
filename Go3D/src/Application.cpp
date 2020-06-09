#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "vendor/stb_image.h"
#include "DebugUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
static Shader* shader = nullptr;

static glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
static glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
static glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);

glm::mat4 LookAt(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 upVector)
{
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

    // Make context current.
    glfwMakeContextCurrent(window);

    // Set callbacks and swap interval.
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
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
    if (!textureData)
        __debugbreak();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);

    // Bind the texture to texture slot=0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Send the texture bounded to slot=0 to the u_texture uniform.
    shader->SetUniform("u_texture", 0); 

    
    // Tranformations
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetUniform("u_projection", projection);

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

    glEnable(GL_DEPTH_TEST);

    // Render loop.
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render.
        glBindVertexArray(va);
        
        
        glm::mat4 view = LookAt(cameraPos, cameraPos + cameraFront, upVector);
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
    // glDeleteBuffers(1, &ib);
    glDeleteVertexArrays(1, &va);
    glDeleteTextures(1, &texture);
    delete shader;
    glfwTerminate();
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera controller.
    const float cameraSpeed = .05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraFront * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += -cameraFront * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += -glm::normalize(glm::cross(cameraFront, upVector)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, upVector)) * cameraSpeed;
        
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
    // Adjust projection matrix's aspect ratio, since it's (most probably)
    // changed as this function is called by glfw.
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 100.f);
    shader->SetUniform("u_projection", projection);
}
