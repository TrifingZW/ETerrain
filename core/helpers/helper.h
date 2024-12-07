//
// Created by TrifingZW on 24-12-7.
//

#pragma once

#include <stb_image.h>

#include <stdexcept>
#include <string>

#include "scene/resources/texture_2d.h"

namespace Helper
{
    inline void LoadTexture2D(Texture2D& texture2D, const std::string& texturePath)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
        if (!data)
            throw std::runtime_error("Failed to load texture: " + texturePath);

        texture2D.Generate(width, height, channels, data);

        // 释放图像内存
        stbi_image_free(data);

        // std::cout << "Loaded texture: " << TextureName << " (ID: " << texture2D.Id << ")\n";
    }
}
