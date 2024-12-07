//
// Created by TrifingZW on 24-11-30.
//

#include "graphics_device.h"

#include <ostream>

#include "graphics_helper.h"
#include "glm/gtx/associated_min_max.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace Graphics;

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
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferManager->EBO);
    short testMatrix[6];
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 4, sizeof(short) * 6, &testMatrix);
    std::cout << testMatrix[0] << std::endl;*/

    _shader.Use();
    _shader.SetMatrix4("projection", observeMatrix);

    texture2D->Bind(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGLAddressMode(samplerState.AddressU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGLAddressMode(samplerState.AddressV));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToOpenGLFilter(samplerState.Filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToOpenGLFilter(samplerState.Filter));

    glBindVertexArray(_bufferManager->VAO);
    glDrawElementsBaseVertex(mode, numVertices, GL_UNSIGNED_SHORT, nullptr, baseVertex);
    // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, 1);
}

void GraphicsDevice::Clear()
{
    Clear(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void GraphicsDevice::Clear(const glm::vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
