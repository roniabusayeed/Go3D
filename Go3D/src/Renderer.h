#pragma once

#include "VertexArray.h"
#include "Shader.h"

class Renderer
{
public:
    Renderer();
    void Draw(const VertexArray& va, const Shader& shader);
};