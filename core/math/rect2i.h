//
// Created by TrifingZW on 25-1-27.
//

#pragma once

#include <string>

#include "vector2i.h"

struct Rect2;

struct Rect2I
{
    union
    {
        struct
        {
            int32_t X, Y;
            int32_t Width, Height;
        };

        struct
        {
            Vector2I Position;
            Vector2I Size;
        };
    };

    Rect2I(): X(), Y(), Width(), Height() {}
    Rect2I(const int32_t x, const int32_t y, const int32_t width, const int32_t height) : X(x), Y(y), Width(width), Height(height) {}
    Rect2I(const Vector2I& pos, const Vector2I& size) : X(pos.X), Y(pos.Y), Width(size.Width), Height(size.Height) {}

    [[nodiscard]] const Vector2I& GetPosition() const { return Position; }
    void SetPosition(const Vector2I& position) { Position = position; }
    [[nodiscard]] const Vector2I& GetSize() const { return Size; }
    void SetSize(const Vector2I& size) { Size = size; }
    [[nodiscard]] int GetArea() const { return Size.Width * Size.Height; }
    [[nodiscard]] Vector2I GetCenter() const { return Position + (Size / 2); }

    [[nodiscard]] bool Intersects(const Rect2I& rect) const
    {
        if (Position.X >= (rect.Position.X + rect.Size.Width))
            return false;
        if ((Position.X + Size.Width) <= rect.Position.X)
            return false;
        if (Position.Y >= (rect.Position.Y + rect.Size.Height))
            return false;
        if ((Position.Y + Size.Height) <= rect.Position.Y)
            return false;
        return true;
    }

    [[nodiscard]] bool Encloses(const Rect2I& rect) const
    {
        return (rect.Position.X >= Position.X) && (rect.Position.Y >= Position.Y) &&
               ((rect.Position.X + rect.Size.X) <= (Position.X + Size.X)) &&
               ((rect.Position.Y + rect.Size.Y) <= (Position.Y + Size.Y));
    }

    [[nodiscard]] bool HasArea() const
    {
        return Size.X > 0 && Size.Y > 0;
    }

    [[nodiscard]] Rect2I Intersection(const Rect2I& rect) const
    {
        Rect2I new_rect = rect;

        if (!Intersects(new_rect))
            return {};

        const Vector2I p_rect_end = rect.Position + rect.Size;
        const Vector2I end = Position + Size;
        new_rect.Position = rect.Position.Max(Position);
        new_rect.Size = p_rect_end.Min(end) - new_rect.Position;

        return new_rect;
    }

    [[nodiscard]] Rect2I Merge(const Rect2I& rect) const
    {
        Rect2I new_rect;
        new_rect.Position = rect.Position.Max(Position);
        new_rect.Size = (rect.Position + rect.Size).Max(Position + Size);
        new_rect.Size = new_rect.Size - new_rect.Position; // Make relative again.
        return new_rect;
    }

    [[nodiscard]] bool HasPoint(const Vector2I& point) const
    {
        if (point.X < Position.X)
            return false;
        if (point.Y < Position.Y)
            return false;

        if (point.X >= (Position.X + Size.X))
            return false;
        if (point.Y >= (Position.Y + Size.Y))
            return false;

        return true;
    }

    [[nodiscard]] Rect2I Grow(const int amount) const
    {
        Rect2I g = *this;
        g.Position.X -= amount;
        g.Position.Y -= amount;
        g.Size.Width += amount * 2;
        g.Size.Height += amount * 2;
        return g;
    }

    [[nodiscard]] Rect2I GrowIndividual(const int left, const int top, const int right, const int bottom) const
    {
        Rect2I g = *this;
        g.Position.X -= left;
        g.Position.Y -= top;
        g.Size.Width += left + right;
        g.Size.Height += top + bottom;
        return g;
    }

    [[nodiscard]] Rect2I Expand(const Vector2I& vector) const
    {
        Rect2I r = *this;
        r.ExpandTo(vector);
        return r;
    }

    void ExpandTo(const Vector2I& vector)
    {
        Vector2I begin = Position;
        Vector2I end = Position + Size;

        if (vector.X < begin.X)
            begin.X = vector.X;
        if (vector.Y < begin.Y)
            begin.Y = vector.Y;

        if (vector.X > end.X)
            end.X = vector.X;
        if (vector.Y > end.Y)
            end.Y = vector.Y;

        Position = begin;
        Size = end - begin;
    }

    [[nodiscard]] Rect2I Abs() const
    {
        return {Position + Size.MinI(0), Size.Abs()};
    }

    void SetEnd(const Vector2I& end)
    {
        Size = end - Position;
    }

    [[nodiscard]] Vector2I GetEnd() const
    {
        return Position + Size;
    }

    bool operator==(const Rect2I& rect) const { return Position == rect.Position && Size == rect.Size; }
    bool operator!=(const Rect2I& rect) const { return Position != rect.Position || Size != rect.Size; }
    explicit operator std::string() const;
    explicit operator Rect2() const;
};
