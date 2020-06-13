#pragma once

class VertexBuffer
{
private:
    unsigned int id;
public:
    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();
    void Bind()const;
    void Unbind()const;
};

