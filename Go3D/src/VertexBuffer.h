#pragma once

class VertexBuffer
{
private:
    unsigned int id;
    size_t size;
public:
    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();
    void Bind()const;
    inline size_t GetSize()const { return size; }
    void Unbind()const;
};

