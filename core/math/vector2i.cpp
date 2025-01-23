//
// Created by TrifingZW on 25-1-22.
//

#include "vector2i.h"

int64_t Vector2I::LengthSquared() const
{
    return x * static_cast<int64_t>(x) + y * static_cast<int64_t>(y);
}

double Vector2I::Length() const
{
    return std::sqrt(static_cast<double>(LengthSquared()));
}

Vector2I Vector2I::Clamp(const Vector2I& min, const Vector2I& max) const
{
    return {
        CLAMP(x, min.x, max.x),
        CLAMP(y, min.y, max.y)
    };
}

Vector2I Vector2I::ClampI(const int32_t min, const int32_t max) const
{
    return {
        CLAMP(x, min, max),
        CLAMP(y, min, max)
    };
}

Vector2I Vector2I::Snapped(const Vector2I& step) const
{
    return {
        static_cast<int32_t>(Math::Snapped(static_cast<float>(x), static_cast<float>(step.x))),
        static_cast<int32_t>(Math::Snapped(static_cast<float>(y), static_cast<float>(step.y)))
    };
}

Vector2I Vector2I::SnappedI(const int32_t step) const
{
    return {
        static_cast<int32_t>(Math::Snapped(static_cast<float>(x), static_cast<float>(step))),
        static_cast<int32_t>(Math::Snapped(static_cast<float>(y), static_cast<float>(step)))
    };
}
