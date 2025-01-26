//
// Created by TrifingZW on 25-1-27.
//

#include "rect2i.h"

#include "rect2.h"

Rect2I::operator std::string() const
{
    return "";
}

Rect2I::operator Rect2() const
{
    return {static_cast<Vector2>(Position), static_cast<Vector2>(Size)};
}
