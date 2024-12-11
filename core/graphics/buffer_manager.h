//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <glad/glad.h>

#include "graphics_enum.h"
#include "graphics_resource.h"

class BufferManager : public GraphicsResource
{
    GLsizeiptr _vertexSize{}, _indexSize{};

public:
    GLuint VAO{}, VBO{}, EBO{};

    BufferManager();
    BufferManager(GLsizeiptr vertexSize, GLsizeiptr indexSize);
    ~BufferManager();

    void Apply() const;

    template<typename T>
    void SetDataPointerEXT(const int offset, const T* data, const int count, const Graphics::SetDataOptions options) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (options == Graphics::SetDataOptions::Discard)
            glBufferData(GL_ARRAY_BUFFER, _vertexSize, data, GL_DYNAMIC_DRAW);
        else
        {
            const auto memoryOffset = static_cast<GLsizeiptr>(offset * sizeof(T));
            const auto memoryLength = static_cast<GLsizeiptr>(count * sizeof(T));

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

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    template<typename T>
    void SetData(const T* data, const size_t memoryLength)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, memoryLength, data, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetIndexPointerEXT(const short* indices, GLsizeiptr size) const;
};
