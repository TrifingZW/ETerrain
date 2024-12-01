//
// Created by TrifingZW on 24-11-30.
//

#include "buffer_manager.h"

#include "graphics_device.h"

BufferManager::BufferManager(
    const GLsizeiptr vertexSize,
    const GLsizeiptr indexSize,
    const GLsizeiptr modelSize
): _vertexSize(vertexSize),
   _indexSize(indexSize),
   _modelSize(modelSize)
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
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 模型矩阵缓冲
    GLuint modelBuffer;
    glGenBuffers(1, &modelBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
    glBufferData(GL_ARRAY_BUFFER, modelSize, nullptr, GL_DYNAMIC_DRAW);

    // 获取着色器中 model uniform 的位置
    constexpr GLuint modelLoc = 2;

    // 假设顶点数据已经传递到 vertex buffer 中
    // 设置模型矩阵的属性
    glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
    glVertexAttribPointer(modelLoc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), static_cast<GLvoid *>(nullptr));
    glVertexAttribDivisor(modelLoc, 1); // 每个实例使用一个不同的矩阵
    glEnableVertexAttribArray(modelLoc);

    GraphicsDevice::ResetBuffer();
}

void BufferManager::SetDataPointerEXT(
    const int offset,
    const PositionTexture4* positionColorTexture4,
    const int count,
    const SetDataOptions options
) const
{
    const auto memoryOffset = static_cast<GLsizeiptr>(offset * 5 * sizeof(float));
    const auto memoryLength = static_cast<GLsizeiptr>(count * 5 * sizeof(float));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (options == SetDataOptions::Discard)
        glBufferData(GL_ARRAY_BUFFER, memoryLength, positionColorTexture4, GL_DYNAMIC_DRAW);
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
}

void BufferManager::SetMatrixPointerEXT(const int offset, const glm::mat4* model, const int count, const SetDataOptions options) const
{
    const auto memoryOffset = static_cast<GLsizeiptr>(offset * sizeof(glm::mat4));
    const auto memoryLength = static_cast<GLsizeiptr>(count * sizeof(glm::mat4));

    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    if (options == SetDataOptions::Discard)
        glBufferData(GL_ARRAY_BUFFER, memoryLength, model, GL_DYNAMIC_DRAW);
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
            memcpy(ptr, model, memoryLength);
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
    }
}
