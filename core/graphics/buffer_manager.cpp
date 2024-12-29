//
// Created by TrifingZW on 24-11-30.
//

#include "buffer_manager.h"
#include "graphics_device.h"

using namespace Graphics;

BufferManager::BufferManager()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

BufferManager::~BufferManager()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    for (const auto& [name, id]: _buffers)
        glDeleteBuffers(1, &id);
}

void BufferManager::Apply() const
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void BufferManager::BindBuffer(const char* name)
{
    if (name)
    {
        if (const auto it = _buffers.find(name); it != _buffers.end())
            glBindBuffer(GL_ARRAY_BUFFER, it->second); // 确保我们按名称绑定正确的缓冲区
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
}

void BufferManager::AddBuffer(const char* name, const GLenum type)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(type, buffer);
    _buffers[name] = buffer;
    glBindBuffer(type, 0); // 创建缓冲区后立即解除绑定
}

void BufferManager::DeleteBuffer(const char* name)
{
    glDeleteBuffers(0, &_buffers[name]);
    _buffers.erase(name);
}

void BufferManager::SetAttribute(const char* name, const int index, const int size, const int stride, const int offset, const int divisor)
{
    glBindVertexArray(VAO);
    BindBuffer(name);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offset));
    glEnableVertexAttribArray(index);
    if (divisor != 0)
        glVertexAttribDivisor(index, divisor);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferManager::SetData(const char* name, const void* data, const size_t memoryLength)
{
    BindBuffer(name);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(memoryLength), data, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferManager::SetIndexPointerEXT(const char* name, const void* indices, const GLsizeiptr size)
{
    if (name)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[name]);
    else
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*
// 在 cpp 文件中显式实例化
template void BufferManager::SetDataPointerEXT<PositionTexture4>(int, const PositionTexture4*, int, Graphics::SetDataOptions) const;

template <typename T>
void BufferManager::SetDataPointerEXT(
    const int offset,
    const T* data,
    const int count,
    const SetDataOptions options
) const
{
    const auto memoryOffset = static_cast<GLsizeiptr>(offset * sizeof(PositionTexture4));
    const auto memoryLength = static_cast<GLsizeiptr>(count * sizeof(PositionTexture4));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (options == SetDataOptions::Discard)
        glBufferData(GL_ARRAY_BUFFER, _vertexSize, data, GL_DYNAMIC_DRAW);
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
            memcpy(ptr, data, memoryLength);
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
    }

    Core::GetGraphicsDevice()->ResetBuffer();
}*/
