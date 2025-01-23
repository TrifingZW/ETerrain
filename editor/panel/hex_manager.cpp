//
// Created by TrifingZW on 24-12-5.
//

#include "hex_manager.h"

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

Vector2 HexManager::GridToPixel(const Vector2I& hex) const
{
    return {
        static_cast<float>(sideLength) + static_cast<float>(hex.x) * (static_cast<float>(sideLength) * (3.0f / 2.0f)),
        innerRadius + static_cast<float>(hex.y) * static_cast<float>(tileHeight) + static_cast<float>(hex.x % 2) * innerRadius
    };
}

Vector2I HexManager::PixelToGrid(const Vector2& pixel) const
{
    Vector2 ret = pixel;
    ret /= Vector2(tileWidth, tileHeight);

    constexpr float overlapping_ratio = 0.75f;
    ret.x /= overlapping_ratio;

    const Vector2 raw_pos = ret;
    if (Math::PosMod(static_cast<long>(Math::Floor(ret.x)), 2))
        ret = Vector2(Math::Floor(ret.x), Math::Floor(ret.y + 0.5f) - 0.5f);
    else
        ret = ret.Floor();

    const Vector2 in_tile_pos = raw_pos - ret;
    const bool in_top_left_triangle = (in_tile_pos - Vector2(0.0f, 0.5f)).Cross(Vector2(1.0f / overlapping_ratio - 1, -0.5f)) > 0;
    const bool in_bottom_left_triangle = (in_tile_pos - Vector2(0.0f, 0.5f)).Cross(Vector2(1.0f / overlapping_ratio - 1, 0.5f)) <= 0;

    auto retI = static_cast<Vector2I>(ret.Floor());
    if (in_top_left_triangle)
        retI += Vector2I(-1, Math::PosMod(static_cast<long>(Math::Floor(ret.x)), 2) ? 0 : -1);
    else if (in_bottom_left_triangle)
        retI += Vector2I(-1, Math::PosMod(static_cast<long>(Math::Floor(ret.x)), 2) ? 1 : 0);

    return retI;
}

Vector2 HexManager::GetStandardPosition(const Vector2& position) const
{
    return GridToPixel(PixelToGrid(position));
}

float HexManager::GetPixelWidth() const
{
    return static_cast<float>(tileWidth) + static_cast<float>(tileWidth) * (3.0f / 4.0f) * static_cast<float>(hexWidth - 1);
}

float HexManager::GetPixelHeight() const
{
    return static_cast<float>(tileHeight) * (3.0f / 2.0f) + static_cast<float>(tileHeight) * static_cast<float>(hexHeight - 1);
}
