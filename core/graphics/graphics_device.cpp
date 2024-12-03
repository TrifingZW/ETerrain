//
// Created by TrifingZW on 24-11-30.
//

#include "graphics_device.h"

#include <iostream>
#include <ostream>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/associated_min_max.hpp"
#include "glm/gtx/string_cast.hpp"

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
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(glm::vec2(1.0f), 0.0f));
    model = scale(model, glm::vec3(glm::vec2(239, 136), 1.0f));

    glBindVertexArray(_bufferManager->VAO);
    _shader.Use();
    _shader.SetMatrix4("model2", model);
    _shader.SetMatrix4("projection", observeMatrix);
    texture2D->Bind(GL_TEXTURE0);
    // glDrawElementsBaseVertex(mode, numVertices, GL_UNSIGNED_SHORT, nullptr, baseVertex);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, 1);

    auto testMatrix = glm::mat4(1.0f);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &testMatrix);
    std::cout << glm::to_string(testMatrix) << std::endl;

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
}
