//
// Created by TrifingZW on 24-12-1.
//

#pragma once
#include <glad/glad.h>
#include "graphics_resource.h"

class RenderTarget: public GraphicsResource
{
public:
    bool depth;
    int width{}, height{};
    GLuint framebuffer = {}, depthRenderbuffer{}, textureBuffer{};

    RenderTarget(int width, int height, bool depth = false);
    ~RenderTarget();

    void Reinitialize(int width, int height, bool depth = false);
};
