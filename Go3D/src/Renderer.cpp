#include "Renderer.h"
#include "DebugUtils.h"

Renderer::Renderer()
{
}

void Renderer::Draw(const VertexArray& va, const Shader& shader)
{
    va.Bind();
    shader.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVertexCount()));
}
