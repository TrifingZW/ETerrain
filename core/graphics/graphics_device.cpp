//
// Created by TrifingZW on 24-11-30.
//

#include "graphics_device.h"

void GraphicsDevice::Clear()
{
    Clear(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void GraphicsDevice::Clear(const glm::vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void GraphicsDevice::ResetBuffer()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GraphicsDevice::ResetRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsDevice::SetRenderTarget(RenderTarget* renderTarget)
{
    _renderTarget = renderTarget;
}
