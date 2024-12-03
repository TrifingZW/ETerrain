//
// Created by TrifingZW on 24-12-1.
//

#include "render_target.h"

#include <iostream>

#include "core.h"
#include "graphics_device.h"

RenderTarget::RenderTarget(const int width, const int height): width(width), height(height)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

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
    glDeleteTextures(GL_TEXTURE_2D, &textureBuffer);
}

void RenderTarget::Reinitialize(const int width, const int height)
{
    this->width = width, this->height = height;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

     Core::GetGraphicsDevice()->ResetRenderTarget();
     Core::GetGraphicsDevice()->ResetTexture();
}
