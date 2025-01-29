//
// Created by TrifingZW on 24-12-6.
//

#pragma once

#include <cstdint>
#include <cmath>
#include <glm/glm.hpp>

#include "core/math/math_defs.h"
#include "core/typedefs.h"

namespace Math
{
    inline constexpr float Epsilon = std::numeric_limits<float>::epsilon();

    //  非负取模
    inline int64_t PosMod(const int64_t p_x, const int64_t p_y)
    {
        int64_t value = p_x % p_y;
        if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0)) {
            value += p_y;
        }
        return value;
    }

    inline float PosModF(const float p_x, const float p_y)
    {
        float value = std::fmod(p_x, p_y);
        if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0))
        {
            value += p_y;
        }
        value += 0.0f;
        return value;
    }

    // 获取小于或等于给定数值的最大整数
    inline double Floor(const double pX) { return floor(pX); }
    inline float Floor(const float pX) { return floorf(pX); }

    inline float Lerp(const float start, const float end, const float interp)
    {
        return start + (end - start) * interp;
    }

    inline bool IsEqualApprox(const float a, const float b, const float tolerance)
    {
        // 首先检查是否完全相等，这是处理“无穷大”值所必需的
        if (a == b)
            return true;
        // 然后检查近似相等
        return abs(a - b) < tolerance;
    }

    inline bool IsEqualApprox(const float a, const float b)
    {
        // 首先检查是否完全相等，这是处理“无穷大”值所必需的
        if (a == b)
            return true;
        // 然后检查近似相等
        float tolerance = static_cast<float>(CMP_EPSILON) * abs(a);
        if (tolerance < static_cast<float>(CMP_EPSILON))
            tolerance = static_cast<float>(CMP_EPSILON);
        return abs(a - b) < tolerance;
    }

    inline bool IsZeroApprox(const float s)
    {
        return abs(s) < static_cast<float>(CMP_EPSILON);
    }

    inline bool IsFinite(const float val) { return isfinite(val); }

    inline float CubicInterpolate(const float from, const float target, const float pre, const float post, const float weight)
    {
        return 0.5f *
               (from * 2.0f +
                (-pre + target) * weight +
                (2.0f * pre - 5.0f * from + 4.0f * target - post) * (weight * weight) +
                (-pre + 3.0f * from - 3.0f * target + post) * (weight * weight * weight));
    }

    inline float CubicInterpolateInTime(
        const float from,
        const float target,
        const float pre,
        const float post,
        const float weight,
        const float time,
        const float preTime,
        const float postTime
    )
    {
        /* 巴里-戈德曼方法 */
        const float t = Lerp(0.0f, time, weight);
        const float a1 = Lerp(pre, from, preTime == 0 ? 0.0f : (t - preTime) / -preTime);
        const float a2 = Lerp(from, target, time == 0 ? 0.5f : t / time);
        const float a3 = Lerp(target, post, postTime - time == 0 ? 1.0f : (t - time) / (postTime - time));
        const float b1 = Lerp(a1, a2, time - preTime == 0 ? 0.0f : (t - preTime) / (time - preTime));
        const float b2 = Lerp(a2, a3, postTime == 0 ? 1.0f : t / postTime);
        return Lerp(b1, b2, time == 0 ? 0.5f : t / time);
    }

    inline float BezierInterpolate(const float start, const float control1, const float control2, const float end, const float time)
    {
        /* 来自维基百科有关贝塞尔曲线的文章的公式。 */
        const float omt = 1.0f - time;
        const float omt2 = omt * omt;
        const float omt3 = omt2 * omt;
        const float t2 = time * time;
        const float t3 = t2 * time;

        return start * omt3 + control1 * omt2 * time * 3.0f + control2 * omt * t2 * 3.0f + end * t3;
    }

    inline float BezierDerivative(const float start, const float control1, const float control2, const float end, const float time)
    {
        /* 来自维基百科有关贝塞尔曲线的文章的公式。 */
        const float omt = 1.0f - time;
        const float omt2 = omt * omt;
        const float t2 = time * time;

        const float d = (control1 - start) * 3.0f * omt2 + (control2 - control1) * 6.0f * omt * time + (end - control2) * 3.0f * t2;
        return d;
    }

    inline float Snapped(float value, const float step)
    {
        if (step != 0)
            value = Floor(value / step + 0.5f) * step;
        return value;
    }
}
