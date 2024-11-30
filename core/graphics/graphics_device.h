//
// Created by TrifingZW on 24-11-30.
//

#pragma once
#include <glad/glad.h>
#include "buffer_manager.h"
#include "render_target.h"
#include "glm/vec4.hpp"
#include "glm/detail/func_packing_simd.inl"


class GraphicsDevice
{
    RenderTarget* _renderTarget = nullptr;
    BufferManager _bufferManager{};

public:
    void SetRenderTarget(RenderTarget* renderTarget);

    static void Clear();
    static void Clear(glm::vec4 color);
    static void ResetBuffer();
    static void ResetRenderTarget();
};
