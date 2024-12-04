//
// Created by TrifingZW on 24-12-1.
//

#include "render_target.h"

#include <iostream>

#include "core.h"
#include "graphics_device.h"

RenderTarget::RenderTarget(const int width, const int height, const bool depth): depth(depth), width(width), height(height)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    if (depth)
    {
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, depthRenderbuffer);
    }

    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    Core::GetGraphicsDevice()->ResetRenderTarget();
    Core::GetGraphicsDevice()->ResetTexture();
}

RenderTarget::~RenderTarget()
{
    glDeleteFramebuffers(GL_FRAMEBUFFER, &framebuffer);
    glDeleteRenderbuffers(GL_RENDERBUFFER, &depthRenderbuffer);
    glDeleteTextures(GL_TEXTURE_2D, &textureBuffer);

    framebuffer = 0;
    depthRenderbuffer = 0;
    textureBuffer = 0;
}

void RenderTarget::Reinitialize(const int width, const int height, const bool depth)
{
    this->width = width;
    this->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // 处理深度缓冲
    if (depth)
    {
        if (!this->depth)
        {
            glGenRenderbuffers(1, &depthRenderbuffer);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        this->depth = true;
    }
    else
    {
        if (this->depth)
        {
            glDeleteRenderbuffers(1, &depthRenderbuffer);
            depthRenderbuffer = 0;
        }
        this->depth = false;
    }

    // 配置纹理
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); // 修改为 GL_RGBA
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

    // 重置设备状态
    Core::GetGraphicsDevice()->ResetRenderTarget();
    Core::GetGraphicsDevice()->ResetTexture();
}
