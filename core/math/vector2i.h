//
// Created by TrifingZW on 25-1-22.
//

#pragma once

#include "vector2.h"
#include "core/math/math_funcs.h"

struct Vector2;

struct Vector2I
{
    static constexpr int AXIS_COUNT = 2;

    enum Axis
    {
        AXIS_X,
        AXIS_Y,
    };

    union
    {
        struct
        {
            union
            {
                int32_t x;
                int32_t width;
            };

            union
            {
                int32_t y;
                int32_t height;
            };
        };

        int32_t coord[2] = {0};
    };

    Vector2I(): x(), y() {}

    Vector2I(const int32_t p_x, const int32_t p_y)
    {
        x = p_x;
        y = p_y;
    }

    [[nodiscard]] Axis MinAxisIndex() const { return x < y ? AXIS_X : AXIS_Y; }
    [[nodiscard]] Axis MaxAxisIndex() const { return x < y ? AXIS_Y : AXIS_X; }

    [[nodiscard]] Vector2I Min(const Vector2I& vector2I) const { return {MIN(x, vector2I.x), MIN(y, vector2I.y)}; }
    [[nodiscard]] Vector2I MinI(const int32_t scalar) const { return {MIN(x, scalar), MIN(y, scalar)}; }
    [[nodiscard]] Vector2I Max(const Vector2I& vector2I) const { return {MAX(x, vector2I.x), MAX(y, vector2I.y)}; }
    [[nodiscard]] Vector2I MaxI(const int32_t scalar) const { return {MAX(x, scalar), MAX(y, scalar)}; }

    [[nodiscard]] double DistanceTo(const Vector2I& target) const { return (target - *this).Length(); }
    [[nodiscard]] int64_t DistanceSquaredTo(const Vector2I& target) const { return (target - *this).LengthSquared(); }

    [[nodiscard]] int64_t LengthSquared() const;
    [[nodiscard]] double Length() const;

    [[nodiscard]] float Aspect() const { return static_cast<float>(width) / static_cast<float>(height); }
    [[nodiscard]] Vector2I Sign() const { return {SIGN(x), SIGN(y)}; }
    [[nodiscard]] Vector2I Abs() const { return {std::abs(x), std::abs(y)}; }
    [[nodiscard]] Vector2I Clamp(const Vector2I& min, const Vector2I& max) const;
    [[nodiscard]] Vector2I ClampI(int32_t min, int32_t max) const;
    [[nodiscard]] Vector2I Snapped(const Vector2I& step) const;
    [[nodiscard]] Vector2I SnappedI(int32_t step) const;

    int32_t& operator[](const int axis) { return coord[axis]; }
    const int32_t& operator[](const int axis) const { return coord[axis]; }

    Vector2I operator+(const Vector2I& vector2I) const { return {x + vector2I.x, y + vector2I.y}; }
    void operator+=(const Vector2I& vector2I) { x += vector2I.x, y += vector2I.y; }
    Vector2I operator-(const Vector2I& vector2I) const { return {x - vector2I.x, y - vector2I.y}; }
    void operator-=(const Vector2I& vector2I) { x -= vector2I.x, y -= vector2I.y; }

    Vector2I operator*(const Vector2I& vector2I) const { return {x * vector2I.x, y * vector2I.y}; }
    Vector2I operator*(const int32_t rvalue) const { return {x * rvalue, y * rvalue}; }

    void operator*=(const int32_t rvalue)
    {
        x *= rvalue;
        y *= rvalue;
    }

    Vector2I operator/(const Vector2I& vector2I) const { return {x / vector2I.x, y / vector2I.y}; }
    Vector2I operator/(const int32_t rvalue) const { return {x / rvalue, y / rvalue}; }

    void operator/=(const int32_t rvalue)
    {
        x /= rvalue;
        y /= rvalue;
    }

    Vector2I operator%(const Vector2I& vector2I) const { return {x % vector2I.x, y % vector2I.y}; }
    Vector2I operator%(const int32_t rvalue) const { return {x % rvalue, y % rvalue}; }

    void operator%=(const int32_t rvalue)
    {
        x %= rvalue;
        y %= rvalue;
    }

    Vector2I operator-() const { return {-x, -y}; }
    bool operator<(const Vector2I& vector2I) const { return (x == vector2I.x) ? (y < vector2I.y) : (x < vector2I.x); }
    bool operator>(const Vector2I& vector2I) const { return (x == vector2I.x) ? (y > vector2I.y) : (x > vector2I.x); }

    bool operator<=(const Vector2I& vector2I) const { return x == vector2I.x ? (y <= vector2I.y) : (x < vector2I.x); }
    bool operator>=(const Vector2I& vector2I) const { return x == vector2I.x ? (y >= vector2I.y) : (x > vector2I.x); }

    bool operator==(const Vector2I& vector2I) const { return x == vector2I.x && y == vector2I.y; }
    bool operator!=(const Vector2I& vector2I) const { return x != vector2I.x || y != vector2I.y; }

    explicit operator Vector2() const { return {static_cast<float>(x), static_cast<float>(y)}; }
    explicit operator glm::ivec2() const { return {x, y}; }

    [[nodiscard]] glm::ivec2 ToGLM() const { return {x, y}; }
};
