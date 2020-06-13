#pragma once
#include <string>

class Texture
{
private:
    unsigned int id;
public:
    Texture(const std::string& texturePath);
    ~Texture();
    void Bind(unsigned int slot = 0);
    void Unbind(unsigned int slot);
};