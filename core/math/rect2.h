//
// Created by TrifingZW on 25-1-27.
//

#pragma once

#include <string>

#include "vector2.h"

struct Rect2I;

struct Rect2
{
    union
    {
        struct
        {
            float X, Y;
            float Width, Height;
        };

        struct
        {
            Vector2 Position;
            Vector2 Size;
        };
    };

    Rect2(): X(), Y(), Width(), Height() {}
    Rect2(const float x, const float y, const float width, const float height) : X(x), Y(y), Width(width), Height(height) {}
    Rect2(const Vector2& pos, const Vector2& size) : X(pos.X), Y(pos.Y), Width(size.Width), Height(size.Height) {}

    [[nodiscard]] bool IsEqualApprox(const Rect2& rect) const;
    [[nodiscard]] bool IsFinite() const;

    [[nodiscard]] const Vector2& GetPosition() const { return Position; }
    void SetPosition(const Vector2& pos) { Position = pos; }
    [[nodiscard]] const Vector2& GetSize() const { return Size; }
    void SetSize(const Vector2& size) { Size = size; }
    [[nodiscard]] float get_area() const { return Size.Width * Size.Height; }
    [[nodiscard]] bool HasArea() const { return Size.X > 0.0f && Size.Y > 0.0f; }
    Vector2 GetCenter() const { return Position + Size * 0.5f; }

    [[nodiscard]] bool Intersects(const Rect2& rect, const bool includeBorders = false) const
    {
        if (includeBorders)
        {
            if (Position.X > rect.Position.X + rect.Size.Width)
                return false;
            if (Position.X + Size.Width < rect.Position.X)
                return false;
            if (Position.Y > rect.Position.Y + rect.Size.Height)
                return false;
            if (Position.Y + Size.Height < rect.Position.Y)
                return false;
        }
        else
        {
            if (Position.X >= rect.Position.X + rect.Size.Width)
                return false;
            if (Position.X + Size.Width <= rect.Position.X)
                return false;
            if (Position.Y >= rect.Position.Y + rect.Size.Height)
                return false;
            if (Position.Y + Size.Height <= rect.Position.Y)
                return false;
        }

        return true;
    }

    [[nodiscard]] float DistanceTo(const Vector2& point) const
    {
        float dist = 0.0;
        bool inside = true;

        if (point.X < Position.X)
        {
            const float d = Position.X - point.X;
            dist = d;
            inside = false;
        }
        if (point.Y < Position.Y)
        {
            const float d = Position.Y - point.Y;
            dist = inside ? d : MIN(dist, d);
            inside = false;
        }
        if (point.X >= Position.X + Size.X)
        {
            const float d = point.X - (Position.X + Size.X);
            dist = inside ? d : MIN(dist, d);
            inside = false;
        }
        if (point.Y >= Position.Y + Size.Y)
        {
            const float d = point.Y - (Position.Y + Size.Y);
            dist = inside ? d : MIN(dist, d);
            inside = false;
        }

        if (inside)
            return 0;

        return dist;
    }

    [[nodiscard]] bool Encloses(const Rect2& rect) const
    {
        return rect.Position.X >= Position.X && rect.Position.Y >= Position.Y &&
               rect.Position.X + rect.Size.X <= Position.X + Size.X &&
               rect.Position.Y + rect.Size.Y <= Position.Y + Size.Y;
    }

    [[nodiscard]] Rect2 Intersection(const Rect2& rect) const
    {
        Rect2 new_rect = rect;

        if (!Intersects(new_rect))
            return {};

        const Vector2 p_rect_end = rect.Position + rect.Size;
        const Vector2 end = Position + Size;

        new_rect.Position = rect.Position.Max(Position);
        new_rect.Size = p_rect_end.Min(end) - new_rect.Position;

        return new_rect;
    }

    [[nodiscard]] Rect2 Merge(const Rect2& rect) const
    {
        Rect2 new_rect;
        new_rect.Position = rect.Position.Min(Position);
        new_rect.Size = (rect.Position + rect.Size).Min(Position + Size);
        new_rect.Size = new_rect.Size - new_rect.Position; // Make relative again.
        return new_rect;
    }

    [[nodiscard]] bool Contains(const Vector2& vector2) const
    {
        if (vector2.X < Position.X)
            return false;
        if (vector2.Y < Position.Y)
            return false;

        if (vector2.X >= (Position.X + Size.X))
            return false;
        if (vector2.Y >= (Position.Y + Size.Y))
            return false;

        return true;
    }

    [[nodiscard]] Rect2 Grow(const float amount) const
    {
        Rect2 g = *this;
        g.GrowBy(amount);
        return g;
    }

    void GrowBy(const float amount)
    {
        Position.X -= amount;
        Position.Y -= amount;
        Size.Width += amount * 2;
        Size.Height += amount * 2;
    }

    [[nodiscard]] Rect2 GrowIndividual(const float left, const float top, const float right, const float bottom) const
    {
        Rect2 g = *this;
        g.Position.X -= left;
        g.Position.Y -= top;
        g.Size.Width += left + right;
        g.Size.Height += top + bottom;
        return g;
    }

    [[nodiscard]] Rect2 Expand(const Vector2& vector) const
    {
        Rect2 r = *this;
        r.ExpandTo(vector);
        return r;
    }

    void ExpandTo(const Vector2& vector)
    {
        Vector2 begin = Position;
        Vector2 end = Position + Size;

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

    [[nodiscard]] Rect2 Abs() const
    {
        return {Position + Size.MinF(0), Size.abs()};
    }

    [[nodiscard]] Rect2 round() const
    {
        return {Position.Round(), Size.Round()};
    }

    Vector2 GetSupport(const Vector2& direction) const
    {
        Vector2 support = Position;
        if (direction.X > 0.0f)
            support.X += Size.X;
        if (direction.Y > 0.0f)
            support.Y += Size.Y;
        return support;
    }

    bool IntersectsFilledPolygon(const Vector2* points, const int pointCount) const
    {
        const Vector2 center = GetCenter();
        int side_plus = 0;
        int side_minus = 0;
        const Vector2 end = Position + Size;

        int i_f = pointCount - 1;
        for (int i = 0; i < pointCount; i++)
        {
            const Vector2& a = points[i_f];
            const Vector2& b = points[i];
            i_f = i;

            Vector2 r = (b - a);
            const float l = r.Length();
            if (l == 0.0f)
                continue;

            // Check inside.
            Vector2 tg = r.Orthogonal();
            if (const float s = tg.Dot(center) - tg.Dot(a); s < 0.0f)
                side_plus++;
            else
                side_minus++;

            // Check ray box.
            r /= l;
            Vector2 ir(1.0f / r.X, 1.0f / r.Y);

            // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
            // r.org is origin of ray
            const Vector2 t13 = (Position - a) * ir;
            const Vector2 t24 = (end - a) * ir;

            const float tmin = MAX(MIN(t13.X, t24.X), MIN(t13.Y, t24.Y));
            const float tmax = MIN(MAX(t13.X, t24.X), MAX(t13.Y, t24.Y));

            // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
            if (tmax < 0 || tmin > tmax || tmin >= l)
                continue;

            return true;
        }

        if (side_plus * side_minus == 0)
            return true; // All inside.
        return false;
    }

    void SetEnd(const Vector2& end)
    {
        Size = end - Position;
    }

    Vector2 GetEnd() const
    {
        return Position + Size;
    }

    bool operator==(const Rect2& rect) const { return Position == rect.Position && Size == rect.Size; }
    bool operator!=(const Rect2& rect) const { return Position != rect.Position || Size != rect.Size; }
    explicit operator std::string() const;
    explicit operator Rect2I() const;
};
