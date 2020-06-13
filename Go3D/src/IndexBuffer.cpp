#include "IndexBuffer.h"
#include <glad/glad.h>
#include "DebugUtils.h"

IndexBuffer::IndexBuffer(const unsigned int* indices, size_t count)
{
    GLCall(glGenBuffers(1, &id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &id));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
