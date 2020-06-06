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
#define TEXTURE_PATH "res/textures/amin.jpg"
#define SHADER_PATH "res/shaders/source.shader"
#else
#define TEXTURE_PATH "amin.jpg"
#define SHADER_PATH "source.shader"
#endif


void ProcessInput(GLFWwindow* window);

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

    // Index data.
    // unsigned int indices[] =
    // {
    //     0, 1, 2,
    //     0, 2, 3,
    // };

    // Generate buffers.
    unsigned int vb, va;
    // unsigned int ib;
    glGenBuffers(1, &vb);
    // glGenBuffers(1, &ib);
    glGenVertexArrays(1, &va);

    // Copy vertex buffer data.
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, 36 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Copy index buffer data.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Pack vertex buffer and vertex buffer layout into vertex array object.
    glBindVertexArray(va);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    
    Shader* shader = new Shader(SHADER_PATH);
    shader->Bind();

    
    // Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
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

    shader->SetUniform("u_texture", 0); // Send the texture bounded to slot=0 to the u_texture uniform.

    
    // Tranformations
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.f));   // Push the camera back (towards me)

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // shader->SetUniform("u_model", model);
    shader->SetUniform("u_view", view);
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
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // model = glm::rotate(model, (float)glfwGetTime()/1000 * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        // shader->SetUniform("u_model", model);
        for (int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = glm::radians((float)glfwGetTime()/5 * (i + 1) * 20.0f);
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
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
}
