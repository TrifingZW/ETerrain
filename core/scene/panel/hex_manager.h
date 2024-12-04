//
// Created by TrifingZW on 24-12-5.
//

#pragma once
#include <cmath>

#include "glm/vec3.hpp"
#include "glm/detail/func_packing_simd.inl"

namespace HexMetrics
{
    constexpr float TileWidth = 148.0f;
    constexpr float TileHeight = 129.0f;
    constexpr float SideLength = 76.0f;
    constexpr float OuterRadius = SideLength;
    constexpr float InnerRadius = SideLength * 0.866025404f;
    constexpr glm::vec3 corners[] = {
        glm::vec3(0.0f, 0.0f, OuterRadius),
        glm::vec3(InnerRadius, 0.0f, 0.5f * OuterRadius),
        glm::vec3(InnerRadius, 0.0f, -0.5f * OuterRadius),
        glm::vec3(0.0f, 0.0f, -OuterRadius),
        glm::vec3(-InnerRadius, 0.0f, -0.5f * OuterRadius),
        glm::vec3(-InnerRadius, 0.0f, 0.5f * OuterRadius),
        glm::vec3(0.0f, 0.0f, OuterRadius)
    };
}

class HexManager
{
public:
    int gridWidth, gridHeight;
    float pixelWidth, pixelHeight;

    HexManager(int gridWidth, int gridHeight);
};
