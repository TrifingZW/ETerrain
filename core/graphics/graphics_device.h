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

    TextureCollection Textures{32};
    SamplerStateCollection SamplerStates{32};
    glm::mat4 ObserveMatrix{};

    GraphicsDevice();

    void SetRenderTarget(RenderTarget* renderTarget);
    void SetBufferManager(BufferManager* bufferManager);

    void DrawPrimitivesIndexed(GLenum mode, int vertexCount);
    void DrawPrimitivesIndexedBase(GLenum mode, int baseVertex, int vertexCount);
    void DrawUserPrimitives(GLenum mode, IVertexType* vertexType, int vertexOffset, int vertexCount);
    void DrawUserPrimitivesIndexed(GLenum mode, IVertexType* vertexType);
    void DrawUserPrimitivesIndexed(GLenum mode, const BufferManager* bufferManager, int count, int instanceCount);

    void ApplyState();
    void ApplyAttribPointer(const VertexDeclaration& vertexDeclaration) const;

    void Clear() const;
    void Clear(glm::vec4 color) const;
    void ResetBuffer() const;
    void ResetTexture() const;
    void ResetRenderTarget() const;
};
