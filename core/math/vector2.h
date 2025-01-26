//
// Created by TrifingZW on 25-1-22.
//

#pragma once

#include <imgui.h>
#include <valarray>

#include "core/math/math_funcs.h"

struct Vector2I;

struct [[nodiscard]] Vector2
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
                float X;
                float Width;
            };

            union
            {
                float Y;
                float Height;
            };
        };

        float coord[2] = {0};
    };

    Vector2(): X(), Y() {}
    explicit Vector2(const float value) { X = value, Y = value; }
    Vector2(const float p_x, const float p_y) { X = p_x, Y = p_y; }

    [[nodiscard]] Axis MinAxisIndex() const { return X < Y ? AXIS_X : AXIS_Y; }
    [[nodiscard]] Axis MaxAxisIndex() const { return X < Y ? AXIS_Y : AXIS_X; }

    void Normalize();
    Vector2 Normalized() const;
    [[nodiscard]] bool IsNormalized() const;

    [[nodiscard]] float Length() const;
    [[nodiscard]] float LengthSquared() const;
    Vector2 LimitLength(float pLen = 1.0) const;

    Vector2 Min(const Vector2& vector2) const { return {MIN(X, vector2.X), MIN(Y, vector2.Y)}; }
    Vector2 MinF(const float scalar) const { return {MIN(X, scalar), MIN(Y, scalar)}; }
    Vector2 Max(const Vector2& vector2) const { return {MAX(X, vector2.X), MAX(Y, vector2.Y)}; }
    Vector2 MaxF(const float scalar) const { return {MAX(X, scalar), MAX(Y, scalar)}; }

    [[nodiscard]] float DistanceTo(const Vector2& vector2) const;
    [[nodiscard]] float DistanceSquaredTo(const Vector2& vector2) const;
    [[nodiscard]] float AngleTo(const Vector2& vector2) const;
    [[nodiscard]] float AngleToPoint(const Vector2& vector2) const;
    Vector2 DirectionTo(const Vector2& target) const;

    [[nodiscard]] float Dot(const Vector2& vector2) const;
    [[nodiscard]] float Cross(const Vector2& vector2) const;
    Vector2 PosMod(float mod) const;
    Vector2 PosModV(const Vector2& modv) const;

    Vector2 Project(const Vector2& target) const;
    Vector2 PlaneProject(float d, const Vector2& target) const;

    Vector2 Lerp(const Vector2& target, float weight) const;
    Vector2 Slerp(const Vector2& target, float weight) const;
    Vector2 CubicInterp(const Vector2& target, const Vector2& preTarget, const Vector2& postTarget, float weight) const;
    Vector2 CubicInterpInTime(
        const Vector2& target,
        const Vector2& preTarget,
        const Vector2& postTarget,
        float weight,
        float time,
        float preTime,
        float postTime
    ) const;
    Vector2 BezierInterp(const Vector2& control1, const Vector2& control2, const Vector2& end, float time) const;
    Vector2 BezierDerivative(const Vector2& control1, const Vector2& control2, const Vector2& end, float time) const;

    Vector2 MoveToward(const Vector2& target, float delta) const;

    Vector2 Slide(const Vector2& normal) const;
    Vector2 Bounce(const Vector2& normal) const;
    Vector2 Reflect(const Vector2& normal) const;

    [[nodiscard]] bool IsEqualApprox(const Vector2& vector2) const;
    [[nodiscard]] bool IsZeroApprox() const;
    [[nodiscard]] bool IsFinite() const;

    [[nodiscard]] float Angle() const;
    static Vector2 FromAngle(float angle);

    Vector2 abs() const { return {std::abs(X), std::abs(Y)}; }

    Vector2 Rotated(float by) const;

    Vector2 Orthogonal() const { return {Y, -X}; }

    Vector2 Sign() const;
    Vector2 Floor() const;
    Vector2 Ceil() const;
    Vector2 Round() const;
    Vector2 Snapped(const Vector2& step) const;
    Vector2 SnappedF(float step) const;
    Vector2 Clamp(const Vector2& min, const Vector2& max) const;
    Vector2 ClampF(float min, float max) const;
    [[nodiscard]] float Aspect() const { return Width / Height; }

    float& operator[](const int axis) { return coord[axis]; }
    const float& operator[](const int axis) const { return coord[axis]; }

    Vector2 operator+(const Vector2& vector2) const { return {X + vector2.X, Y + vector2.Y}; }
    void operator+=(const Vector2& vector2) { X += vector2.X, Y += vector2.Y; }
    Vector2 operator-(const Vector2& vector2) const { return {X - vector2.X, Y - vector2.Y}; }
    void operator-=(const Vector2& vector2) { X -= vector2.X, Y -= vector2.Y; }

    Vector2 operator*(const Vector2& vector2) const { return {X * vector2.X, Y * vector2.Y}; }
    Vector2 operator*(const float rvalue) const { return {X * rvalue, Y * rvalue}; }
    void operator*=(const float rvalue) { X *= rvalue, Y *= rvalue; }
    void operator*=(const Vector2& rvalue) { *this = *this * rvalue; }

    Vector2 operator/(const Vector2& vector2) const { return {X / vector2.X, Y / vector2.Y}; }
    Vector2 operator/(const float rvalue) const { return {X / rvalue, Y / rvalue}; }
    void operator/=(const float rvalue) { X /= rvalue, Y /= rvalue; }
    void operator/=(const Vector2& rvalue) { *this = *this / rvalue; }

    Vector2 operator-() const { return {-X, -Y}; }

    bool operator==(const Vector2& vector2) const { return X == vector2.X && Y == vector2.Y; }
    bool operator!=(const Vector2& vector2) const { return X != vector2.X || Y != vector2.Y; }

    bool operator<(const Vector2& vector2) const { return X == vector2.X ? (Y < vector2.Y) : (X < vector2.X); }
    bool operator>(const Vector2& vector2) const { return X == vector2.X ? (Y > vector2.Y) : (X > vector2.X); }
    bool operator<=(const Vector2& vector2) const { return X == vector2.X ? (Y <= vector2.Y) : (X < vector2.X); }
    bool operator>=(const Vector2& vector2) const { return X == vector2.X ? (Y >= vector2.Y) : (X > vector2.X); }

    explicit operator Vector2I() const;
    explicit operator glm::vec2() const;
    explicit operator ImVec2() const;

    [[nodiscard]] glm::vec2 ToGLM() const { return {X, Y}; }
    [[nodiscard]] ImVec2 ToImVec2() const { return {X, Y}; }
};

// 使用隐式转换解决 LLVM 问题所需的乘法运算符
// 对于不应该的整数，改为 Vector2i

inline Vector2 operator*(const float p_scalar, const Vector2& p_vec)
{
    return p_vec * p_scalar;
}

inline Vector2 operator*(const double p_scalar, const Vector2& p_vec)
{
    return p_vec * static_cast<float>(p_scalar);
}

inline Vector2 operator*(const int32_t p_scalar, const Vector2& p_vec)
{
    return p_vec * static_cast<float>(p_scalar);
}

inline Vector2 operator*(const int64_t p_scalar, const Vector2& p_vec)
{
    return p_vec * static_cast<float>(p_scalar);
}
