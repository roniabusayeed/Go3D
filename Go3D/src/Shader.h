#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderSource
{
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
};

class Shader
{
private:
    unsigned int m_id;
    mutable std::unordered_map<std::string, int> m_uniformLocationCache;
public:
    Shader(const std::string& filePath);
    ~Shader();
    void Bind()const;
    void Unbind()const;

    void SetUniform(const std::string& uniformName, int value);
    void SetUniform(const std::string& uniformName, float value);
    void SetUniform(const std::string& uniformName, const glm::mat4& matrix);
    void SetUniform(const std::string& uniformName, const glm::vec3& vector);
private:
    static ShaderSource ParseShader(const std::string& filePath);
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& uniformName)const;
};

