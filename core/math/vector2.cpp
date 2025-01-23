//
// Created by TrifingZW on 25-1-22.
//

#include "vector2.h"

#include "vector2i.h"

void Vector2::Normalize()
{
    if (float l = x * x + y * y; l != 0)
    {
        l = std::sqrt(l);
        x /= l;
        y /= l;
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
    return std::sqrt(x * x + y * y);
}

float Vector2::LengthSquared() const
{
    return x * x + y * y;
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
    return std::sqrt((x - vector2.x) * (x - vector2.x) + (y - vector2.y) * (y - vector2.y));
}

float Vector2::DistanceSquaredTo(const Vector2& vector2) const
{
    return (x - vector2.x) * (x - vector2.x) + (y - vector2.y) * (y - vector2.y);
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
    Vector2 ret(target.x - x, target.y - y);
    ret.Normalize();
    return ret;
}

float Vector2::Dot(const Vector2& vector2) const
{
    return x * vector2.x + y * vector2.y;
}

float Vector2::Cross(const Vector2& vector2) const
{
    return x * vector2.y - y * vector2.x;
}

Vector2 Vector2::PosMod(const float mod) const
{
    return {Math::PosModF(x, mod), Math::PosModF(y, mod)};
}

Vector2 Vector2::PosModV(const Vector2& modv) const
{
    return {Math::PosModF(x, modv.x), Math::PosModF(y, modv.y)};
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
    res.x = Math::Lerp(res.x, target.x, weight);
    res.y = Math::Lerp(res.y, target.y, weight);
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
    res.x = Math::CubicInterpolate(res.x, target.x, preTarget.x, postTarget.x, weight);
    res.y = Math::CubicInterpolate(res.y, target.y, preTarget.y, postTarget.y, weight);
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
    res.x = Math::CubicInterpolateInTime(res.x, target.x, preTarget.x, postTarget.x, weight, time, preTime, postTime);
    res.y = Math::CubicInterpolateInTime(res.y, target.y, preTarget.y, postTarget.y, weight, time, preTime, postTime);
    return res;
}

Vector2 Vector2::BezierInterp(const Vector2& control1, const Vector2& control2, const Vector2& end, const float time) const
{
    Vector2 res = *this;
    res.x = Math::BezierInterpolate(res.x, control1.x, control2.x, end.x, time);
    res.y = Math::BezierInterpolate(res.y, control1.y, control2.y, end.y, time);
    return res;
}

Vector2 Vector2::BezierDerivative(const Vector2& control1, const Vector2& control2, const Vector2& end, const float time) const
{
    Vector2 res = *this;
    res.x = Math::BezierDerivative(res.x, control1.x, control2.x, end.x, time);
    res.y = Math::BezierDerivative(res.y, control1.y, control2.y, end.y, time);
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
    return Math::IsEqualApprox(x, vector2.x) && Math::IsEqualApprox(y, vector2.y);
}

bool Vector2::IsZeroApprox() const
{
    return Math::IsZeroApprox(x) && Math::IsZeroApprox(y);
}

bool Vector2::IsFinite() const
{
    return Math::IsFinite(x) && Math::IsFinite(y);
}

float Vector2::Angle() const
{
    return std::atan2(y, x);
}

Vector2 Vector2::FromAngle(const float angle)
{
    return {std::cos(angle), std::sin(angle)};
}

Vector2 Vector2::Rotated(const float by) const
{
    const float sine = std::sin(by);
    const float cosi = std::cos(by);
    return {x * cosi - y * sine, x * sine + y * cosi};
}

Vector2 Vector2::Sign() const
{
    return {SIGN(x), SIGN(y)};
}

Vector2 Vector2::Floor() const
{
    return {Math::Floor(x), Math::Floor(y)};
}

Vector2 Vector2::Ceil() const
{
    return {std::ceil(x), std::ceil(y)};
}

Vector2 Vector2::Round() const
{
    return {std::round(x), std::round(y)};
}

Vector2 Vector2::Snapped(const Vector2& step) const
{
    return {Math::Snapped(x, step.x), Math::Snapped(y, step.y)};
}

Vector2 Vector2::SnappedF(const float step) const
{
    return {Math::Snapped(x, step), Math::Snapped(y, step)};
}

Vector2 Vector2::Clamp(const Vector2& min, const Vector2& max) const
{
    return {CLAMP(x, min.x, max.x), CLAMP(y, min.y, max.y)};
}

Vector2 Vector2::ClampF(const float min, const float max) const
{
    return {CLAMP(x, min, max), CLAMP(y, min, max)};
}

Vector2::operator Vector2I() const { return {static_cast<int32_t>(x), static_cast<int32_t>(y)}; }
Vector2::operator glm::vec<2, float>() const { return {x, y}; }
Vector2::operator ImVec2() const { return {x, y}; }
