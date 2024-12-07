//
// Created by TrifingZW on 24-11-22.
//

#include "texture_2d.h"

void Texture2D::Generate(const int width, const int height, const int channels, const unsigned char* data)
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

void Texture2D::SetPixelColor(const int x, const int y, const unsigned char data) const
{
    SetRangePixelColor(x, y, 1, 1, &data);
}

void Texture2D::SetRangePixelColor(const int x, const int y, const int width, const int height, const unsigned char* data) const
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
