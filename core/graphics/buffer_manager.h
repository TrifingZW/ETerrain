//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <glad/glad.h>

#include "graphics_enum.h"
#include "graphics_structure.h"
#include "glm/fwd.hpp"

class BufferManager
{
    GLsizeiptr _vertexSize{}, _indexSize{}, _modelSize{};

public:
    GLuint VAO{}, VBO{}, EBO{}, MBO{};

    BufferManager(GLsizeiptr vertexSize, GLsizeiptr indexSize, GLsizeiptr modelSize);

    void SetDataPointerEXT(int offset, const PositionTexture4* positionColorTexture4, int count, SetDataOptions options) const;
    void SetMatrixPointerEXT(int offset, const glm::mat4* model, int count, SetDataOptions options) const;
};
