#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int id;
public:
    VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
    ~VertexArray();
    void Bind()const;
    void Unbind()const;
};