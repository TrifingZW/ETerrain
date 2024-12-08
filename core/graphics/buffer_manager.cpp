//
// Created by TrifingZW on 24-11-30.
//

#include "buffer_manager.h"

#include "core.h"
#include "graphics_device.h"

using namespace Graphics;

BufferManager::BufferManager(
    const GLsizeiptr vertexSize,
    const GLsizeiptr indexSize
): _vertexSize(vertexSize),
   _indexSize(indexSize)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, nullptr,GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 4,GL_FLOAT,GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2,GL_FLOAT,GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Core::GetGraphicsDevice()->ResetBuffer();
}

void BufferManager::SetDataPointerEXT(
    const int offset,
    const PositionTexture4* positionColorTexture4,
    const int count,
    const SetDataOptions options
) const
{
    const auto memoryOffset = static_cast<GLsizeiptr>(offset * sizeof(PositionTexture4));
    const auto memoryLength = static_cast<GLsizeiptr>(count * sizeof(PositionTexture4));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (options == SetDataOptions::Discard)
        glBufferData(GL_ARRAY_BUFFER, _vertexSize, positionColorTexture4, GL_DYNAMIC_DRAW);
    else
    {
        // 映射缓冲区并更新部分数据
        void* ptr = glMapBufferRange(
            GL_ARRAY_BUFFER,
            memoryOffset,
            memoryLength,
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT
        );
        if (ptr)
        {
            memcpy(ptr, positionColorTexture4, memoryLength);
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
    }

    Core::GetGraphicsDevice()->ResetBuffer();
}

void BufferManager::SetIndexPointerEXT(const short* indices, const GLsizeiptr size) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices,GL_STATIC_DRAW);
}
