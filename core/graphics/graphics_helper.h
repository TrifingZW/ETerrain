//
// Created by TrifingZW on 24-12-7.
//

#pragma once

#include <glad/glad.h>

#include "graphics_enum.h"

namespace Graphics
{
    inline GLint ToOpenGLAddressMode(const TextureAddressMode mode)
    {
        switch (mode)
        {
            case TextureAddressMode::Wrap:
                return GL_REPEAT;
            case TextureAddressMode::Clamp:
                return GL_CLAMP_TO_EDGE;
            case TextureAddressMode::Mirror:
                return GL_MIRRORED_REPEAT;
            default:
                return GL_REPEAT; // 默认使用GL_REPEAT
        }
    }

    inline GLint ToOpenGLFilter(const TextureFilter filter)
    {
        switch (filter)
        {
            case TextureFilter::Linear:
                return GL_LINEAR;
            case TextureFilter::Point:
                return GL_NEAREST;
            // TODO: 添加Anisotropic过滤器
            case TextureFilter::Anisotropic:
                return GL_LINEAR;
            default:
                return GL_LINEAR; // 默认使用GL_REPEAT
        }
    }
}
