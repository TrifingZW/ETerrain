//
// Created by TrifingZW on 24-12-1.
//

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "core/math/vector2.h"

struct Rect
{
    Rect(const float x, const float y, const float width, const float height): X(x), Y(y), Width(width), Height(height) {}
    float X{}, Y{}, Width{}, Height{};

    Vector2 GetPosition() const { return {X, Y}; }
    Vector2 GetSize() const { return {Width, Height}; }
};

struct Color
{
    float R{}, G{}, B{}, A{};
    Color(const float red, const float green, const float blue, const float alpha): R(red), G(green), B(blue), A(alpha) {}
};

struct PositionTexture4
{
    glm::vec4 Position0{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec2 TextureCoordinate0{0.0f, 0.0f};

    glm::vec4 Position1{1.0f, 0.0f, 0.0f, 1.0f};
    glm::vec2 TextureCoordinate1{1.0f, 0.0f};

    glm::vec4 Position2{1.0f, 1.0f, 0.0f, 1.0f};
    glm::vec2 TextureCoordinate2{1.0f, 1.0f};

    glm::vec4 Position3{0.0f, 1.0f, 0.0f, 1.0f};
    glm::vec2 TextureCoordinate3{0.0f, 1.0f};

    static constexpr PositionTexture4 Default() { return {}; }
};
