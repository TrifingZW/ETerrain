//
// Created by TrifingZW on 24-12-5.
//

#include "hex_manager.h"

#include <glm/common.hpp>

#include "core/math/math_funcs.h"

HexManager::HexManager(const int hexWidth, const int hexHeight, const float sideLength)
    : hexWidth(hexWidth),
      hexHeight(hexHeight),
      sideLength(sideLength)
{
    tileWidth = sideLength * 2.0f;
    tileHeight = sideLength * 2.0f * (sqrt(3.0f) / 2.0f);
    outerRadius = sideLength;
    innerRadius = tileHeight / 2.0f;
}

glm::vec2 HexManager::HexToPixel(const glm::ivec2& hex) const
{
    return {
        static_cast<float>(sideLength) + (static_cast<float>(hex.x) * (static_cast<float>(sideLength) * (3.0f / 2.0f))),
        innerRadius + static_cast<float>(hex.y) * static_cast<float>(tileHeight) + static_cast<float>(hex.x % 2) * innerRadius
    };
}

glm::ivec2 HexManager::PixelToHex(const glm::vec2& pixel) const
{
    glm::vec2 ret = pixel;
    ret /= glm::vec2(tileWidth, tileHeight);

    constexpr float overlapping_ratio = 0.75f;
    ret.x /= overlapping_ratio;

    const glm::vec2 raw_pos = ret;
    if (Math::PosMod(static_cast<long>(Math::floor(ret.x)), 2))
        ret = glm::vec2(Math::floor(ret.x), Math::floor(ret.y + 0.5) - 0.5);
    else
        ret = floor(ret);

    const glm::vec2 in_tile_pos = raw_pos - ret;
    const bool in_top_left_triangle = Math::Cross(in_tile_pos - glm::vec2(0.0, 0.5), glm::vec2(1.0 / overlapping_ratio - 1, -0.5)) > 0;
    const bool in_bottom_left_triangle = Math::Cross(in_tile_pos - glm::vec2(0.0, 0.5), glm::vec2(1.0 / overlapping_ratio - 1, 0.5)) <= 0;

    ret = floor(ret);
    if (in_top_left_triangle)
        ret += glm::ivec2(-1, Math::PosMod(static_cast<long>(Math::floor(ret.x)), 2) ? 0 : -1);
    else if (in_bottom_left_triangle)
        ret += glm::ivec2(-1, Math::PosMod(static_cast<long>(Math::floor(ret.x)), 2) ? 1 : 0);

    return {ret.x, ret.y};
}

float HexManager::GetPixelWidth() const
{
    return static_cast<float>(tileWidth) + static_cast<float>(tileWidth) * (3.0f / 4.0f) * static_cast<float>(hexWidth - 1);
}

float HexManager::GetPixelHeight() const
{
    return static_cast<float>(tileHeight) * (3.0f / 2.0f) + static_cast<float>(tileHeight) * static_cast<float>(hexHeight - 1);
}
