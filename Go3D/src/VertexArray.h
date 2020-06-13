#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int id;
    size_t vertexCount;
public:
    VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
    ~VertexArray();
    inline size_t GetVertexCount()const { return vertexCount; }
    void Bind()const;
    void Unbind()const;
};