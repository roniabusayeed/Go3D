#include "Texture.h"
#include <glad/glad.h>
#include "vendor/stb_image.h"
#include "DebugUtils.h"

Texture::Texture(const std::string& texturePath)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imgWidth, imgHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureData = stbi_load(texturePath.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);
    ASSERT(textureData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind(unsigned int slot)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
