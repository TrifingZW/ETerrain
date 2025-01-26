//
// Created by TrifingZW on 25-1-22.
//

#include "vector2i.h"

int64_t Vector2I::LengthSquared() const
{
    return X * static_cast<int64_t>(X) + Y * static_cast<int64_t>(Y);
}

double Vector2I::Length() const
{
    return std::sqrt(static_cast<double>(LengthSquared()));
}

Vector2I Vector2I::Clamp(const Vector2I& min, const Vector2I& max) const
{
    return {
        CLAMP(X, min.X, max.X),
        CLAMP(Y, min.Y, max.Y)
    };
}

Vector2I Vector2I::ClampI(const int32_t min, const int32_t max) const
{
    return {
        CLAMP(X, min, max),
        CLAMP(Y, min, max)
    };
}

Vector2I Vector2I::Snapped(const Vector2I& step) const
{
    return {
        static_cast<int32_t>(Math::Snapped(static_cast<float>(X), static_cast<float>(step.X))),
        static_cast<int32_t>(Math::Snapped(static_cast<float>(Y), static_cast<float>(step.Y)))
    };
}

Vector2I Vector2I::SnappedI(const int32_t step) const
{
    return {
        static_cast<int32_t>(Math::Snapped(static_cast<float>(X), static_cast<float>(step))),
        static_cast<int32_t>(Math::Snapped(static_cast<float>(Y), static_cast<float>(step)))
    };
}
