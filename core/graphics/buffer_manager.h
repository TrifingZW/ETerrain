//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <glad/glad.h>

#include "graphics_enum.h"
#include "graphics_resource.h"
#include "graphics_structure.h"

class BufferManager : public GraphicsResource
{
    GLsizeiptr _vertexSize{}, _indexSize{};

public:
    GLuint VAO{}, VBO{}, EBO{};

    BufferManager(GLsizeiptr vertexSize, GLsizeiptr indexSize);

    void SetDataPointerEXT(int offset, const PositionTexture4* positionColorTexture4, int count, Graphics::SetDataOptions options) const;
    void SetIndexPointerEXT(const short* indices, GLsizeiptr size) const;
};
