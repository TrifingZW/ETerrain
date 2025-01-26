//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include <string>
#include <glad/glad.h>

#include "core/graphics/graphics_structure.h"
#include "core/math/vector2.h"

class Texture2D
{
public:
    GLuint Id{};
    GLsizei Width = 0;
    GLsizei Height = 0;
    GLint InternalFormat = GL_RGB;
    GLint ImageFormat = GL_RGB;
    GLint WrapS = GL_REPEAT;
    GLint WrapT = GL_REPEAT;
    GLint FilterMin = GL_LINEAR;
    GLint FilterMax = GL_LINEAR;
    // 从图像数据生成纹理

    Texture2D();
    ~Texture2D();

    void Generate(int width, int height, int channels = 4, const void* data = nullptr);
    // 将纹理绑定为当前活动的 GL_TEXTURE_2D 纹理对象
    void Bind(GLenum target) const;
    void SetData(const unsigned char* data) const;
    void SetPixelColor(int x, int y, const void* data) const;
    void SetRangePixelColor(int x, int y, int width, int height, const void* data) const;

    Vector2 GetSize() const;

    void SavePNG(const char* filename) const;
};
