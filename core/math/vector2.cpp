//
// Created by TrifingZW on 25-1-22.
//

#include "vector2.h"

#include "vector2i.h"

void Vector2::Normalize()
{
    if (float l = X * X + Y * Y; l != 0)
    {
        l = std::sqrt(l);
        X /= l;
        Y /= l;
    }
}

Vector2 Vector2::Normalized() const
{
    Vector2 v = *this;
    v.Normalize();
    return v;
}

bool Vector2::IsNormalized() const
{
    return Math::IsEqualApprox(LengthSquared(), 1, UNIT_EPSILON);
}

float Vector2::Length() const
{
    return std::sqrt(X * X + Y * Y);
}

float Vector2::LengthSquared() const
{
    return X * X + Y * Y;
}

Vector2 Vector2::LimitLength(const float pLen) const
{
    const float l = Length();
    Vector2 v = *this;
    if (l > 0 && pLen < l)
    {
        v /= l;
        v *= pLen;
    }
    return v;
}

float Vector2::DistanceTo(const Vector2& vector2) const
{
    return std::sqrt((X - vector2.X) * (X - vector2.X) + (Y - vector2.Y) * (Y - vector2.Y));
}

float Vector2::DistanceSquaredTo(const Vector2& vector2) const
{
    return (X - vector2.X) * (X - vector2.X) + (Y - vector2.Y) * (Y - vector2.Y);
}

float Vector2::AngleTo(const Vector2& vector2) const
{
    return std::atan2(Cross(vector2), Dot(vector2));
}

float Vector2::AngleToPoint(const Vector2& vector2) const
{
    return (vector2 - *this).Angle();
}

Vector2 Vector2::DirectionTo(const Vector2& target) const
{
    Vector2 ret(target.X - X, target.Y - Y);
    ret.Normalize();
    return ret;
}

float Vector2::Dot(const Vector2& vector2) const
{
    return X * vector2.X + Y * vector2.Y;
}

float Vector2::Cross(const Vector2& vector2) const
{
    return X * vector2.Y - Y * vector2.X;
}

Vector2 Vector2::PosMod(const float mod) const
{
    return {Math::PosModF(X, mod), Math::PosModF(Y, mod)};
}

Vector2 Vector2::PosModV(const Vector2& modv) const
{
    return {Math::PosModF(X, modv.X), Math::PosModF(Y, modv.Y)};
}

Vector2 Vector2::Project(const Vector2& target) const
{
    return target * (Dot(target) / target.LengthSquared());
}

Vector2 Vector2::PlaneProject(const float d, const Vector2& target) const
{
    return target - *this * (Dot(target) - d);
}

Vector2 Vector2::Lerp(const Vector2& target, const float weight) const
{
    Vector2 res = *this;
    res.X = Math::Lerp(res.X, target.X, weight);
    res.Y = Math::Lerp(res.Y, target.Y, weight);
    return res;
}

Vector2 Vector2::Slerp(const Vector2& target, const float weight) const
{
    const float start_length_sq = LengthSquared();
    const float end_length_sq = target.LengthSquared();
    if (start_length_sq == 0.0f || end_length_sq == 0.0f)
    {
        // 零长度向量没有角度，所以我们能做的最好的就是 lerp 或抛出错误
        return Lerp(target, weight);
    }
    const float start_length = std::sqrt(start_length_sq);
    const float result_length = Math::Lerp(start_length, std::sqrt(end_length_sq), weight);
    const float angle = AngleTo(target);
    return Rotated(angle * weight) * (result_length / start_length);
}

Vector2 Vector2::CubicInterp(const Vector2& target, const Vector2& preTarget, const Vector2& postTarget, const float weight) const
{
    Vector2 res = *this;
    res.X = Math::CubicInterpolate(res.X, target.X, preTarget.X, postTarget.X, weight);
    res.Y = Math::CubicInterpolate(res.Y, target.Y, preTarget.Y, postTarget.Y, weight);
    return res;
}

Vector2 Vector2::CubicInterpInTime(
    const Vector2& target,
    const Vector2& preTarget,
    const Vector2& postTarget,
    const float weight,
    const float time,
    const float preTime,
    const float postTime
) const
{
    Vector2 res = *this;
    res.X = Math::CubicInterpolateInTime(res.X, target.X, preTarget.X, postTarget.X, weight, time, preTime, postTime);
    res.Y = Math::CubicInterpolateInTime(res.Y, target.Y, preTarget.Y, postTarget.Y, weight, time, preTime, postTime);
    return res;
}

Vector2 Vector2::BezierInterp(const Vector2& control1, const Vector2& control2, const Vector2& end, const float time) const
{
    Vector2 res = *this;
    res.X = Math::BezierInterpolate(res.X, control1.X, control2.X, end.X, time);
    res.Y = Math::BezierInterpolate(res.Y, control1.Y, control2.Y, end.Y, time);
    return res;
}

Vector2 Vector2::BezierDerivative(const Vector2& control1, const Vector2& control2, const Vector2& end, const float time) const
{
    Vector2 res = *this;
    res.X = Math::BezierDerivative(res.X, control1.X, control2.X, end.X, time);
    res.Y = Math::BezierDerivative(res.Y, control1.Y, control2.Y, end.Y, time);
    return res;
}

Vector2 Vector2::MoveToward(const Vector2& target, const float delta) const
{
    const Vector2 v = *this;
    const Vector2 vd = target - v;
    const float len = vd.Length();
    return len <= delta || len < CMP_EPSILON ? target : v + vd / len * delta;
}

Vector2 Vector2::Slide(const Vector2& normal) const
{
    return *this - normal * Dot(normal);
}

Vector2 Vector2::Bounce(const Vector2& normal) const
{
    return -Reflect(normal);
}

Vector2 Vector2::Reflect(const Vector2& normal) const
{
    return 2.0f * normal * Dot(normal) - *this;
}

bool Vector2::IsEqualApprox(const Vector2& vector2) const
{
    return Math::IsEqualApprox(X, vector2.X) && Math::IsEqualApprox(Y, vector2.Y);
}

bool Vector2::IsZeroApprox() const
{
    return Math::IsZeroApprox(X) && Math::IsZeroApprox(Y);
}

bool Vector2::IsFinite() const
{
    return Math::IsFinite(X) && Math::IsFinite(Y);
}

float Vector2::Angle() const
{
    return std::atan2(Y, X);
}

Vector2 Vector2::FromAngle(const float angle)
{
    return {std::cos(angle), std::sin(angle)};
}

Vector2 Vector2::Rotated(const float by) const
{
    const float sine = std::sin(by);
    const float cosi = std::cos(by);
    return {X * cosi - Y * sine, X * sine + Y * cosi};
}

Vector2 Vector2::Sign() const
{
    return {SIGN(X), SIGN(Y)};
}

Vector2 Vector2::Floor() const
{
    return {Math::Floor(X), Math::Floor(Y)};
}

Vector2 Vector2::Ceil() const
{
    return {std::ceil(X), std::ceil(Y)};
}

Vector2 Vector2::Round() const
{
    return {std::round(X), std::round(Y)};
}

Vector2 Vector2::Snapped(const Vector2& step) const
{
    return {Math::Snapped(X, step.X), Math::Snapped(Y, step.Y)};
}

Vector2 Vector2::SnappedF(const float step) const
{
    return {Math::Snapped(X, step), Math::Snapped(Y, step)};
}

Vector2 Vector2::Clamp(const Vector2& min, const Vector2& max) const
{
    return {CLAMP(X, min.X, max.X), CLAMP(Y, min.Y, max.Y)};
}

Vector2 Vector2::ClampF(const float min, const float max) const
{
    return {CLAMP(X, min, max), CLAMP(Y, min, max)};
}

Vector2::operator Vector2I() const { return {static_cast<int32_t>(X), static_cast<int32_t>(Y)}; }
Vector2::operator glm::vec<2, float>() const { return {X, Y}; }
Vector2::operator ImVec2() const { return {X, Y}; }
