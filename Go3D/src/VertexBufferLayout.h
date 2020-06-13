#pragma once

#include <vector>
#include <glad/glad.h>
#include "DebugUtils.h"

struct Attribute
{
    size_t Count;
    unsigned int Type;
    unsigned int Normalized;

    inline static size_t GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:  return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }

        // Add support for more types.
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
    std::vector<Attribute> attributes;
    size_t stride;
public:
    VertexBufferLayout();
    inline size_t GetStride()const { return stride; }
    inline const std::vector<Attribute>& GetAttributes()const { return attributes; }

    template<typename T>
    inline void Push(size_t count)
    {
        static_assert(false);
    }

    template<>
    inline void Push<float>(size_t count)
    {
        attributes.push_back({ count, GL_FLOAT, GL_FALSE });
        stride += count * Attribute::GetSizeOfType(GL_FLOAT);
    }

    // Add support for more attribute field types.
};

