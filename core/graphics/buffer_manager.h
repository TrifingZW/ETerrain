//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <map>
#include <glad/glad.h>

#include "graphics_enum.h"
#include "graphics_resource.h"

class BufferManager : public GraphicsResource
{
    GLsizeiptr _vertexSize{}, _indexSize{};

public:
    std::map<std::string, GLuint> _buffers;
    GLuint VAO{}, VBO{}, EBO{};

    BufferManager();
    BufferManager(GLsizeiptr vertexSize, GLsizeiptr indexSize);
    ~BufferManager();

    void Apply() const;
    void BindBuffer(const char* name = nullptr);
    void AddBuffer(const char* name, GLenum type);
    void DeleteBuffer(const char* name);
    void SetAttribute(const char* name, int index, int size, int stride, int offset, int divisor = 0);

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

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetData(const char* name, const void* data, size_t memoryLength);
    void SetIndexPointerEXT(const char* name, const void* indices, GLsizeiptr size);
};
