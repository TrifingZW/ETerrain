//
// Created by TrifingZW on 24-11-22.
//

#include "texture_2d.h"

#include <iostream>
#include <stb_image_write.h>

Texture2D::Texture2D() = default;

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &Id);
}

void Texture2D::Generate(const int width, const int height, const int channels, const void* data)
{
    Width = width;
    Height = height;
    InternalFormat = ImageFormat = channels == 4 ? GL_RGBA : GL_RGB;
    // 创建纹理
    glGenTextures(1, &Id);
    glBindTexture(GL_TEXTURE_2D, Id);

    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, ImageFormat, GL_UNSIGNED_BYTE, data);
    // 设置纹理环绕和过滤模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMax);
    // 生成 mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    // 解除纹理绑定
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetData(const unsigned char* data) const
{
    glBindTexture(GL_TEXTURE_2D, Id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, ImageFormat, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetPixelColor(const int x, const int y, const void* data) const
{
    SetRangePixelColor(x, y, 1, 1, data);
}

void Texture2D::SetRangePixelColor(const int x, const int y, const int width, const int height, const void* data) const
{
    // 更新纹理的部分区域
    glBindTexture(GL_TEXTURE_2D, Id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture2D::Bind(const GLenum target) const
{
    glActiveTexture(target);
    glBindTexture(GL_TEXTURE_2D, Id);
}

Vector2 Texture2D::GetSize() const { return {static_cast<float>(Width), static_cast<float>(Height)}; }

void Texture2D::SavePNG(const char* filename) const
{
    // 创建一个用于保存像素数据的缓冲区
    auto* pixels = new unsigned char[Width * Height * 4]; // RGBA 每个像素4个字节

    // 读取纹理的像素数据
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // 使用 stb_image_write 保存为 PNG
    if (!stbi_write_png(filename, Width, Height, 4, pixels, Width * 4))
    {
        // 错误处理
        std::cerr << "Failed to save PNG!" << std::endl;
    }

    // 释放像素数据缓冲区
    delete[] pixels;
}
