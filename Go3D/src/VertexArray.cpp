#include "VertexArray.h"
#include <glad/glad.h>
#include "DebugUtils.h"

VertexArray::VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    vertexCount = vb.GetSize() / layout.GetStride();

    GLCall(glGenVertexArrays(1, &id));
    GLCall(glBindVertexArray(id));
    vb.Bind();
    auto attributes = layout.GetAttributes();
    size_t offset = 0;
    for (int i = 0, n = attributes.size(); i < n; i++)
    {
        GLCall(glVertexAttribPointer(i, attributes[i].Count, attributes[i].Type,
            attributes[i].Normalized, layout.GetStride(), (const void*)(offset)));
        GLCall(glEnableVertexAttribArray(i));

        offset += attributes[i].Count * Attribute::GetSizeOfType(attributes[i].Type);
    }
    vb.Unbind();    // vao doesn't keep track of vb unbind calls. 
                    // But keeps track of ib unbind calls. So, it's safe
                    // to unbind vb while vao is still bound.
    GLCall(glBindVertexArray(0));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &id));
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(id));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
