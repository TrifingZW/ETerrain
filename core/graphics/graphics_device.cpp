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

void GraphicsDevice::DrawIndexedPrimitives(const GLenum mode, const int numVertices)
{
    ApplyState();

    _bufferManager->Apply();
    glDrawElements(mode, numVertices, GL_UNSIGNED_SHORT, nullptr);

    ResetBuffer();
}

void GraphicsDevice::DrawIndexedPrimitivesBase(const GLenum mode, const int baseVertex, const int numVertices)
{
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferManager->EBO);
    short testMatrix[6];
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 4, sizeof(short) * 6, &testMatrix);
    std::cout << testMatrix[0] << std::endl;*/

    ApplyState();


    _bufferManager->Apply();
    glDrawElementsBaseVertex(mode, numVertices, GL_UNSIGNED_SHORT, nullptr, baseVertex);

    ResetBuffer();
    // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, 1);
}

void GraphicsDevice::DrawUserPrimitives(const GLenum mode, IVertexType* vertexType, const size_t vertexOffset, const size_t vertexCount)
{
    ApplyState();

    _userBufferManager.SetData(nullptr, vertexType->GetVertexDataPtr(), vertexType->GetVertexMemorySize() * vertexCount);
    _userBufferManager.Apply();

    // 计算图形的数量
    const GLsizei drawCount = vertexCount / 6; // 每个图形由 6 个顶点组成

    // 创建数组来存储起始顶点索引和每个图形的顶点数
    const auto first = new GLint[drawCount];
    const auto count = new GLsizei[drawCount];

    // 填充 first 和 count 数组
    for (int i = 0; i < drawCount; ++i)
    {
        first[i] = i * 6; // 每个图形的起始顶点是 6 的倍数
        count[i] = 6; // 每个图形有 6 个顶点
    }

    ApplyAttribPointer(vertexType->GetVertexDeclaration());
    glMultiDrawArrays(mode, first, count, drawCount);

    // 释放内存
    delete[] first;
    delete[] count;
}

void GraphicsDevice::DrawUserPrimitivesIndexed(const GLenum mode, IVertexType* vertexType)
{
    ApplyState();

    _userBufferManager.SetData(nullptr, vertexType->GetVertexDataPtr(), vertexType->GetVertexDataMemorySize());
    _userBufferManager.SetIndexPointerEXT(nullptr, vertexType->GetIndicesDataPtr(), static_cast<GLsizei>(vertexType->GetIndicesDataMemorySize()));
    _userBufferManager.Apply();

    ApplyAttribPointer(vertexType->GetVertexDeclaration());

    glDrawElementsInstanced(mode, 6, GL_UNSIGNED_SHORT, nullptr, 3);
}

void GraphicsDevice::DrawUserPrimitivesIndexed(const GLenum mode, const BufferManager* bufferManager, const int count, const int instanceCount)
{
    ApplyState();
    glBindVertexArray(bufferManager->VAO);
    glDrawArraysInstanced(mode, 0, count, instanceCount);
}

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

void GraphicsDevice::ApplyAttribPointer(const VertexDeclaration& vertexDeclaration)
{
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
