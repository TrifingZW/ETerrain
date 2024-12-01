//
// Created by TrifingZW on 24-11-30.
//

#include "graphics_device.h"

#include "glm/gtx/associated_min_max.hpp"

void GraphicsDevice::SetRenderTarget(RenderTarget* renderTarget)
{
    _renderTarget = renderTarget;
    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->framebuffer);
    glViewport(0, 0, renderTarget->width, renderTarget->height);
}

void GraphicsDevice::SetBufferManager(BufferManager* bufferManager) { _bufferManager = bufferManager; }


void GraphicsDevice::DrawIndexedPrimitives(
    const GLenum mode,
    const int baseVertex,
    const int numVertices
) const
{
    _shader.Use();
    _shader.SetMatrix4("projection", observeMatrix);
    texture2D->Bind(GL_TEXTURE0);
    glBindVertexArray(_bufferManager->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferManager->EBO);
    glDrawElementsBaseVertex(mode, numVertices,GL_UNSIGNED_INT, nullptr, baseVertex);
}

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

void GraphicsDevice::ResetTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GraphicsDevice::ResetRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
