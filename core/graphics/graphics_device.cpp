//
// Created by TrifingZW on 24-11-30.
//

#include "graphics_device.h"

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
)
{
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferManager->EBO);
    short testMatrix[6];
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 4, sizeof(short) * 6, &testMatrix);
    std::cout << testMatrix[0] << std::endl;*/

    ApplyState();

    glBindVertexArray(_bufferManager->VAO);
    glDrawElementsBaseVertex(mode, numVertices, GL_UNSIGNED_SHORT, nullptr, baseVertex);

    // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, 1);
}

template<typename T>
void GraphicsDevice::DrawUserPrimitives(GLenum mode, T* vertices, int vertexOffset, int primitiveCount) const {}

void GraphicsDevice::ApplyState()
{
    if (currentShaderId == 0)
        _shader.Apply();
    if (currentShaderId == _shader.shaderId)
    {
        _shader.SetMatrix4("uTransform", observeMatrix);
        _shader.SetInt("image", 0);
    }

    if (currentShaderId != _shader.shaderId)
        currentShaderId = 0;

    for (int index = 0; index < textures.slots; index++)
    {
        if (textures[index])
        {
            const SamplerState samplerState = samplerStates[index];
            textures[index]->Bind(GL_TEXTURE0 + index);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGLAddressMode(samplerState.AddressU));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGLAddressMode(samplerState.AddressV));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToOpenGLFilter(samplerState.Filter));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToOpenGLFilter(samplerState.Filter));
            textures[index] = nullptr;
        }
    }
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
