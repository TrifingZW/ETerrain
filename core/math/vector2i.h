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
                int32_t X;
                int32_t Width;
            };

            union
            {
                int32_t Y;
                int32_t Height;
            };
        };

        int32_t coord[2] = {0};
    };

    Vector2I(): X(), Y() {}

    Vector2I(const int32_t p_x, const int32_t p_y)
    {
        X = p_x;
        Y = p_y;
    }

    [[nodiscard]] Axis MinAxisIndex() const { return X < Y ? AXIS_X : AXIS_Y; }
    [[nodiscard]] Axis MaxAxisIndex() const { return X < Y ? AXIS_Y : AXIS_X; }

    [[nodiscard]] Vector2I Min(const Vector2I& vector2I) const { return {MIN(X, vector2I.X), MIN(Y, vector2I.Y)}; }
    [[nodiscard]] Vector2I MinI(const int32_t scalar) const { return {MIN(X, scalar), MIN(Y, scalar)}; }
    [[nodiscard]] Vector2I Max(const Vector2I& vector2I) const { return {MAX(X, vector2I.X), MAX(Y, vector2I.Y)}; }
    [[nodiscard]] Vector2I MaxI(const int32_t scalar) const { return {MAX(X, scalar), MAX(Y, scalar)}; }

    [[nodiscard]] double DistanceTo(const Vector2I& target) const { return (target - *this).Length(); }
    [[nodiscard]] int64_t DistanceSquaredTo(const Vector2I& target) const { return (target - *this).LengthSquared(); }

    [[nodiscard]] int64_t LengthSquared() const;
    [[nodiscard]] double Length() const;

    [[nodiscard]] float Aspect() const { return static_cast<float>(Width) / static_cast<float>(Height); }
    [[nodiscard]] Vector2I Sign() const { return {SIGN(X), SIGN(Y)}; }
    [[nodiscard]] Vector2I Abs() const { return {std::abs(X), std::abs(Y)}; }
    [[nodiscard]] Vector2I Clamp(const Vector2I& min, const Vector2I& max) const;
    [[nodiscard]] Vector2I ClampI(int32_t min, int32_t max) const;
    [[nodiscard]] Vector2I Snapped(const Vector2I& step) const;
    [[nodiscard]] Vector2I SnappedI(int32_t step) const;

    int32_t& operator[](const int axis) { return coord[axis]; }
    const int32_t& operator[](const int axis) const { return coord[axis]; }

    Vector2I operator+(const Vector2I& vector2I) const { return {X + vector2I.X, Y + vector2I.Y}; }
    void operator+=(const Vector2I& vector2I) { X += vector2I.X, Y += vector2I.Y; }
    Vector2I operator-(const Vector2I& vector2I) const { return {X - vector2I.X, Y - vector2I.Y}; }
    void operator-=(const Vector2I& vector2I) { X -= vector2I.X, Y -= vector2I.Y; }

    Vector2I operator*(const Vector2I& vector2I) const { return {X * vector2I.X, Y * vector2I.Y}; }
    Vector2I operator*(const int32_t rvalue) const { return {X * rvalue, Y * rvalue}; }

    void operator*=(const int32_t rvalue)
    {
        X *= rvalue;
        Y *= rvalue;
    }

    Vector2I operator/(const Vector2I& vector2I) const { return {X / vector2I.X, Y / vector2I.Y}; }
    Vector2I operator/(const int32_t rvalue) const { return {X / rvalue, Y / rvalue}; }

    void operator/=(const int32_t rvalue)
    {
        X /= rvalue;
        Y /= rvalue;
    }

    Vector2I operator%(const Vector2I& vector2I) const { return {X % vector2I.X, Y % vector2I.Y}; }
    Vector2I operator%(const int32_t rvalue) const { return {X % rvalue, Y % rvalue}; }

    void operator%=(const int32_t rvalue)
    {
        X %= rvalue;
        Y %= rvalue;
    }

    Vector2I operator-() const { return {-X, -Y}; }
    bool operator<(const Vector2I& vector2I) const { return (X == vector2I.X) ? (Y < vector2I.Y) : (X < vector2I.X); }
    bool operator>(const Vector2I& vector2I) const { return (X == vector2I.X) ? (Y > vector2I.Y) : (X > vector2I.X); }

    bool operator<=(const Vector2I& vector2I) const { return X == vector2I.X ? (Y <= vector2I.Y) : (X < vector2I.X); }
    bool operator>=(const Vector2I& vector2I) const { return X == vector2I.X ? (Y >= vector2I.Y) : (X > vector2I.X); }

    bool operator==(const Vector2I& vector2I) const { return X == vector2I.X && Y == vector2I.Y; }
    bool operator!=(const Vector2I& vector2I) const { return X != vector2I.X || Y != vector2I.Y; }

    explicit operator Vector2() const { return {static_cast<float>(X), static_cast<float>(Y)}; }
    explicit operator glm::ivec2() const { return {X, Y}; }

    [[nodiscard]] glm::ivec2 ToGLM() const { return {X, Y}; }
};
