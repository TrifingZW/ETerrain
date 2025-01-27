//
// Created by TrifingZW on 24-11-30.
//

#include "graphics_device.h"
#include "graphics_helper.h"

using namespace Graphics;

GraphicsDevice::GraphicsDevice() = default;

void GraphicsDevice::SetRenderTarget(RenderTarget* renderTarget)
{
    _renderTarget = renderTarget;
    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->framebuffer);
    glViewport(0, 0, renderTarget->width, renderTarget->height);
}

void GraphicsDevice::SetBufferManager(BufferManager* bufferManager) { _bufferManager = bufferManager; }

void GraphicsDevice::DrawPrimitivesIndexed(const GLenum mode, const int vertexCount)
{
    ApplyState();

    _bufferManager->Apply();
    glDrawElements(mode, vertexCount, GL_UNSIGNED_SHORT, nullptr);

    ResetBuffer();
}

void GraphicsDevice::DrawPrimitivesIndexedBase(const GLenum mode, const int baseVertex, const int vertexCount)
{
    ApplyState();

    _bufferManager->Apply();
    glDrawElementsBaseVertex(mode, vertexCount, GL_UNSIGNED_SHORT, nullptr, baseVertex);

    ResetBuffer();
}

void GraphicsDevice::DrawUserPrimitives(const GLenum mode, IVertexType* vertexType, const int vertexOffset, const int vertexCount)
{
    ApplyState();

    _userBufferManager.SetData(nullptr, vertexType->GetVertexDataPtr(), vertexType->GetVertexMemorySize() * vertexCount);
    ApplyAttribPointer(vertexType->GetVertexDeclaration());

    _userBufferManager.Apply();
    glDrawArrays(mode, vertexOffset, vertexCount);

    ResetBuffer();
}

void GraphicsDevice::DrawUserPrimitivesIndexed(const GLenum mode, IVertexType* vertexType)
{
    ApplyState();

    _userBufferManager.SetData(nullptr, vertexType->GetVertexDataPtr(), vertexType->GetVertexDataMemorySize());
    _userBufferManager.SetIndexPointerEXT(nullptr, vertexType->GetIndicesDataPtr(), static_cast<GLsizei>(vertexType->GetIndicesDataMemorySize()));
    ApplyAttribPointer(vertexType->GetVertexDeclaration());

    _userBufferManager.Apply();
    glDrawElementsInstanced(mode, 6, GL_UNSIGNED_SHORT, nullptr, 3);

    ResetBuffer();
}

void GraphicsDevice::DrawUserPrimitivesIndexed(const GLenum mode, const BufferManager* bufferManager, const int count, const int instanceCount)
{
    ApplyState();

    glBindVertexArray(bufferManager->VAO);
    glDrawArraysInstanced(mode, 0, count, instanceCount);

    ResetBuffer();
}

void GraphicsDevice::ApplyState()
{
    if (currentShaderId == 0)
        _shader.Apply();
    if (currentShaderId == _shader.shaderId)
    {
        _shader.SetMatrix4("uTransform", ObserveMatrix);
        _shader.SetInt("Image", 0);
    }

    if (currentShaderId != _shader.shaderId)
        currentShaderId = 0;

    for (int index = 0; index < Textures.slots; index++)
    {
        if (Textures[index])
        {
            const SamplerState samplerState = SamplerStates[index];
            Textures[index]->Bind(GL_TEXTURE0 + index);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGLAddressMode(samplerState.AddressU));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGLAddressMode(samplerState.AddressV));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToOpenGLFilter(samplerState.Filter));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToOpenGLFilter(samplerState.Filter));
            Textures[index] = nullptr;
        }
    }
}

void GraphicsDevice::ApplyAttribPointer(const VertexDeclaration& vertexDeclaration) const
{
    _userBufferManager.Apply();

    for (size_t index = 0; index < vertexDeclaration.NumElements; index++)
    {
        const VertexElement element = vertexDeclaration.Elements[index];
        glVertexAttribPointer(
            element.Index,
            element.Size,
            GL_FLOAT,
            GL_FALSE,
            vertexDeclaration.Stride,
            reinterpret_cast<void *>(element.Offset * element.Type)
        );
        glEnableVertexAttribArray(element.Index);
    }

    ResetBuffer();
}

void GraphicsDevice::Clear() const
{
    Clear(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void GraphicsDevice::Clear(const glm::vec4 color) const
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsDevice::ResetBuffer() const
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GraphicsDevice::ResetTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GraphicsDevice::ResetRenderTarget() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
