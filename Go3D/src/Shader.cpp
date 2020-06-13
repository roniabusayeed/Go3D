#include "Shader.h"
#include <glad/glad.h>
#include "DebugUtils.h"
#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& filePath)
{
    ShaderSource shaderSource = ParseShader(filePath);
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, shaderSource.vertexShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.fragmentShaderSource);
    m_id = glCreateProgram();
    GLCall(glAttachShader(m_id, vs));
    GLCall(glAttachShader(m_id, fs));
    GLCall(glLinkProgram(m_id));

    // Delete intermediates.
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    // Handle shader linking errors.
    int success;
    GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
    if (!success)
    {
        int length;
        GLCall(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length));

        char* infoLog = new char[length];
        GLCall(glGetProgramInfoLog(m_id, length, nullptr, infoLog));
        std::cout << "[Shader Linking Error] " << infoLog << std::endl;
        delete[] infoLog;

        ASSERT(false);
    }
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_id));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_id));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform(const std::string& uniformName, int value)
{
    int location = GetUniformLocation(uniformName);
    if (location != -1)
    {
        GLCall(glUniform1i(location, value));
    }
    else
        ASSERT(false);  // Invalid uniform name.
}

void Shader::SetUniform(const std::string& uniformName, float value)
{
    int location = GetUniformLocation(uniformName);
    if (location != -1)
    {
        GLCall(glUniform1f(location, value));
    }
    else
        ASSERT(false);  // Invalid uniform name.
}

void Shader::SetUniform(const std::string& uniformName, const glm::mat4& matrix)
{
    int location = GetUniformLocation(uniformName);
    if (location != -1)
    {
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
    else ASSERT(false);     // Invalid uniform name.
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec3& vector)
{
    int location = GetUniformLocation(uniformName);
    if (location != -1)
    {
        GLCall(glUniform3f(location, vector.x, vector.y, vector.z));
    }
    else ASSERT(false);     // Invalid uniform name.
}

ShaderSource Shader::ParseShader(const std::string& filePath)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX_SHADER = 0,
        FRAGMENT_SHADER = 1,
    };

    std::ifstream stream(filePath);
    ASSERT(stream.is_open());

    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2];
    std::string line;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX_SHADER;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT_SHADER;

            continue;
        }

        if (type != ShaderType::NONE)
            ss[(int)type] << line << '\n';
    }

    stream.close();
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(shader, 1, &src, nullptr));
    GLCall(glCompileShader(shader));

    // Handle shader compilation errors.
    int success;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        int length;
        GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
        char* infoLog = new char[length];
        GLCall(glGetShaderInfoLog(shader, length, nullptr, infoLog));

        std::string shaderType;
        if (type == GL_VERTEX_SHADER)
            shaderType = "Vertex";
        else if (type == GL_FRAGMENT_SHADER)
            shaderType = "Fragment";

        std::cout << "[" << shaderType << " Shader Compilation Error] " <<
            infoLog << std::endl;
        delete[] infoLog;

        ASSERT(false);
        return 0;
    }

    return shader;
}

int Shader::GetUniformLocation(const std::string& uniformName) const
{
    // Use cache
    if (m_uniformLocationCache.count(uniformName))
        return m_uniformLocationCache[uniformName];

    int location = glGetUniformLocation(m_id, uniformName.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << uniformName << "' doesn't exist!" << std::endl;
    m_uniformLocationCache[uniformName] = location;
    return location;
}
