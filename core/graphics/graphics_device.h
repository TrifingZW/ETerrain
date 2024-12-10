//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <glad/glad.h>
#include "buffer_manager.h"
#include "render_target.h"
#include "glm/detail/func_packing_simd.inl"
#include "scene/resources/shader.h"
#include "graphics_resource.h"
#include "sampler_state_collection.h"
#include "texture_collection.h"
#include "vertex_type.h"

class GraphicsDevice : public GraphicsResource
{
    Shader _shader{};
    RenderTarget* _renderTarget = nullptr;
    BufferManager* _bufferManager = nullptr;
    BufferManager _userBufferManager{};

public:
    GLuint currentShaderId = 0;

    TextureCollection textures{32};
    SamplerStateCollection samplerStates{32};
    glm::mat4 observeMatrix{};

    GraphicsDevice();

    void SetRenderTarget(RenderTarget* renderTarget);
    void SetBufferManager(BufferManager* bufferManager);
    void DrawIndexedPrimitives(
        GLenum mode,
        int baseVertex,
        int numVertices
    );
    void DrawUserPrimitives(
        GLenum mode,
        IVertexType* vertices,
        int vertexOffset,
        int vertexCount
    );

    void ApplyState();
    void ApplyAttribPointer(const VertexDeclaration& vertexDeclaration);

    void Clear();
    void Clear(glm::vec4 color);
    void ResetBuffer();
    void ResetTexture();
    void ResetRenderTarget();
};
