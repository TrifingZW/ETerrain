//
// Created by TrifingZW on 25-1-27.
//

#include "rect2.h"

#include "rect2i.h"
#include "vector2i.h"

bool Rect2::IsEqualApprox(const Rect2& rect) const
{
    return Position.IsEqualApprox(rect.Position) && Size.IsEqualApprox(rect.Size);
}

bool Rect2::IsFinite() const
{
    return Position.IsFinite() && Size.IsFinite();
}

Rect2::operator std::string() const
{
    return "";
}

Rect2::operator Rect2I() const
{
    return {static_cast<Vector2I>(Position), static_cast<Vector2I>(Size)};
}
