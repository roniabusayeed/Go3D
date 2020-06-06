#include "DebugUtils.h"
#include <glad/glad.h>
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* func, const char* file, int line)
{
    int count = 0;
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error " << error << "] "
            << func << " " << file << ":" << line << std::endl;
        count++;
    }
    return !count;
}
