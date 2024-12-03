//
// Created by TrifingZW on 24-12-1.
//

#pragma once
#include <glad/glad.h>


class RenderTarget
{
public:
    int width{}, height{};
    GLuint framebuffer = {}, textureBuffer{};

    RenderTarget(int width, int height);
    ~RenderTarget();

   void Reinitialize(int width, int height);
};
