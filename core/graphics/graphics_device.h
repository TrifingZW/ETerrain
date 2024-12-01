//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <glad/glad.h>
#include "buffer_manager.h"
#include "render_target.h"
#include "glm/detail/func_packing_simd.inl"
#include "scene/resources/shader.h"
#include "scene/resources/texture_2d.h"

class GraphicsDevice
{
    Shader _shader{};
    RenderTarget* _renderTarget = nullptr;
    BufferManager* _bufferManager = nullptr;

public:
    Texture2D* texture2D = nullptr;
    glm::mat4 modelMatrix{};
    glm::mat4 observeMatrix{};

    void SetRenderTarget(RenderTarget* renderTarget);
    void SetBufferManager(BufferManager* bufferManager);
    void DrawIndexedPrimitives(
        GLenum mode,
        int baseVertex,
        int numVertices
    ) const;

    static void Clear();
    static void Clear(glm::vec4 color);
    static void ResetBuffer();
    static void ResetTexture();
    static void ResetRenderTarget();
};
