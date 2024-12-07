//
// Created by TrifingZW on 24-12-5.
//

#pragma once
#include <cmath>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/detail/func_packing_simd.inl"

namespace HexMetrics
{
    constexpr float TileWidth = 148.0f;
    constexpr float TileHeight = 129.0f;

    constexpr float SideLength = 1.0f;
    constexpr float OuterRadius = SideLength;
    constexpr float InnerRadius = SideLength * 0.866025404f;
    /*constexpr glm::vec3 Corners[] = {
        glm::vec3(0.0f, 0.0f, OuterRadius),
        glm::vec3(InnerRadius, 0.0f, 0.5f * OuterRadius),
        glm::vec3(InnerRadius, 0.0f, -0.5f * OuterRadius),
        glm::vec3(0.0f, 0.0f, -OuterRadius),
        glm::vec3(-InnerRadius, 0.0f, -0.5f * OuterRadius),
        glm::vec3(-InnerRadius, 0.0f, 0.5f * OuterRadius),
        glm::vec3(0.0f, 0.0f, OuterRadius)
    };*/
    constexpr glm::vec3 Corners[] = {
        glm::vec3(OuterRadius, 0.0f, 0.0f),
        glm::vec3(OuterRadius * 0.5f, 0.0f, -InnerRadius),
        glm::vec3(-OuterRadius * 0.5f, 0.0f, -InnerRadius),
        glm::vec3(-OuterRadius, 0.0f, 0.0f),
        glm::vec3(-OuterRadius * 0.5f, 0.0f, InnerRadius),
        glm::vec3(OuterRadius * 0.5f, 0.0f, InnerRadius),
    };
}

class HexManager
{
public:
    float hexWidth, hexHeight;
    int tileWidth, tileHeight, sideLength;
    float outerRadius, innerRadius;
    HexManager(float hexWidth, float hexHeight, int tileWidth, int tileHeight);

    [[nodiscard]] glm::vec2 HexToPixel(const glm::ivec2& hex) const;
    [[nodiscard]] glm::ivec2 PixelToHex(const glm::vec2& pixel) const;
    [[nodiscard]] float GetPixelWidth() const;
    [[nodiscard]] float GetPixelHeight() const;
};
