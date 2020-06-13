#include "VertexBuffer.h"
#include <glad/glad.h>
#include "DebugUtils.h"

VertexBuffer::VertexBuffer(const void* data, size_t size)
    : size(size)
{
    GLCall(glGenBuffers(1, &id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &id));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
