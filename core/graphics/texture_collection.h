//
// Created by TrifingZW on 24-12-8.
//

#pragma once

#include <stdexcept>

#include "scene/resources/texture_2d.h"

class TextureCollection
{
public:
    int slots = 0;
    explicit TextureCollection(int slots);

    // 重载operator[]，使得可以通过索引访问纹理
    Texture2D*& operator[](const int index)
    {
        if (index < 0 || index >= slots)
            throw std::out_of_range("Index out of range");

        return textures[index];
    }

    const Texture2D* operator[](const int index) const
    {
        if (index < 0 || index >= slots)
            throw std::out_of_range("Index out of range");

        return textures[index];
    }

private:
    Texture2D** textures = nullptr;
};
