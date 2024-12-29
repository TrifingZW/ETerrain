//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <glad/glad.h>

#include "scene/resources/shader.h"
#include "buffer_manager.h"
#include "render_target.h"
#include "graphics_resource.h"
#include "sampler_state_collection.h"
#include "texture_collection.h"
#include "vertex_type.h"

class GraphicsDevice : public GraphicsResource
{
    Shader _shader{};
    BufferManager _userBufferManager{};

    RenderTarget* _renderTarget = nullptr;
    BufferManager* _bufferManager = nullptr;

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
        IVertexType* vertexType,
        size_t vertexOffset,
        size_t vertexCount
    );
    void DrawUserPrimitivesIndexed(
        GLenum mode,
        IVertexType* vertexType
    );
    void DrawUserPrimitivesIndexed(
        GLenum mode,
        const BufferManager* bufferManager,
        int count,
        int instanceCount
    );

    void ApplyState();
    void ApplyAttribPointer(const VertexDeclaration& vertexDeclaration);

    void Clear();
    void Clear(glm::vec4 color);
    void ResetBuffer();
    void ResetTexture();
    void ResetRenderTarget();
};
