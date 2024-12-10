//
// Created by TrifingZW on 24-12-5.
//

#pragma once
#include <cmath>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/detail/func_packing_simd.inl"
#include "graphics/vertex_declaration.h"
#include "graphics/vertex_type.h"

namespace HexMetrics
{
    constexpr float TileWidth = 148.0f;
    constexpr float TileHeight = 129.0f;

    constexpr float SideLength = 1.0f;
    constexpr float OuterRadius = SideLength;
    constexpr float InnerRadius = SideLength * 0.866025404f;
    constexpr glm::vec3 CornersXZ[] = {
        glm::vec3(0.0f, 0.0f, OuterRadius),
        glm::vec3(InnerRadius, 0.0f, 0.5f * OuterRadius),
        glm::vec3(InnerRadius, 0.0f, -0.5f * OuterRadius),
        glm::vec3(0.0f, 0.0f, -OuterRadius),
        glm::vec3(-InnerRadius, 0.0f, -0.5f * OuterRadius),
        glm::vec3(-InnerRadius, 0.0f, 0.5f * OuterRadius),
        glm::vec3(0.0f, 0.0f, OuterRadius)
    };

    constexpr glm::vec3 Corners[] = {
        glm::vec3(-OuterRadius, 0.0f, 0.0f),
        glm::vec3(-0.5f * OuterRadius, InnerRadius, 0.0f),
        glm::vec3(0.5f * OuterRadius, InnerRadius, 0.0f),
        glm::vec3(OuterRadius, 0.0f, 0.0f),
        glm::vec3(0.5f * OuterRadius, -InnerRadius, 0.0f),
        glm::vec3(-0.5f * OuterRadius, -InnerRadius, 0.0f),
    };
}

struct HexVertexData
{
    glm::vec4 Position;
    glm::vec2 Uv;
};

class HexVertexType final : public IVertexType
{
public:
    int count{};
    std::vector<HexVertexData> PHexVertexData{};

    explicit HexVertexType(const int count): count(count) {}
    ~HexVertexType() override = default;

    VertexDeclaration GetVertexDeclaration() override
    {
        return {
            VertexElement(0, 0, 4, 4),
            VertexElement(1, 4, 2, 4)
        };
    }

    int GetVertexCount() override { return count; }

    void* GetVertexDataPtr() override { return PHexVertexData.data(); }

    int GetVertexMemorySize() override { return sizeof(HexVertexData); }

    int GetDataMemorySize() override { return static_cast<int>(count * sizeof(HexVertexData)); };
};

class HexManager
{
public:
    int hexWidth, hexHeight;
    float tileWidth, tileHeight, sideLength;
    float outerRadius, innerRadius;
    HexManager(int hexWidth, int hexHeight, float sideLength);

    [[nodiscard]] glm::vec2 HexToPixel(const glm::ivec2& hex) const;
    [[nodiscard]] glm::ivec2 PixelToHex(const glm::vec2& pixel) const;
    [[nodiscard]] float GetPixelWidth() const;
    [[nodiscard]] float GetPixelHeight() const;
};
