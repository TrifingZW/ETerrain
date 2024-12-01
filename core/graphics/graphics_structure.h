//
// Created by TrifingZW on 24-12-1.
//

#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct Rect
{
    float X{}, Y{}, Width{}, Height{};
};

struct Color
{
    float R{}, G{}, B{}, A{};
    Color(const float red, const float green, const float blue, const float alpha): R(red), G(green), B(blue), A(alpha) {}
};

struct PositionTexture4
{
    glm::vec3 Position0{0.0f, 0.0f, 0.0f};
    glm::vec2 TextureCoordinate0{0.0f, 0.0f};

    glm::vec3 Position1{1.0f, 0.0f, 0.0f};
    glm::vec2 TextureCoordinate1{1.0f, 0.0f};

    glm::vec3 Position2{1.0f, 1.0f, 0.0f};
    glm::vec2 TextureCoordinate2{1.0f, 1.0f};

    glm::vec3 Position3{0.0f, 1.0f, 0.0f};
    glm::vec2 TextureCoordinate3{0.0f, 1.0f};
};
