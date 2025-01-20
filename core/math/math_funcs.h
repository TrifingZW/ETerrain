//
// Created by TrifingZW on 24-12-6.
//

#pragma once

#include <cstdint>
#include <cmath>

namespace Math
{
    constexpr float Epsilon = std::numeric_limits<float>::epsilon();

    inline int64_t PosMod(const int64_t p_x, const int64_t p_y)
    {
        int64_t value = p_x % p_y;
        if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0))
        {
            value += p_y;
        }
        return value;
    }

    inline double Floor(const double pX) { return ::floor(pX); }
    inline float Floor(const float pX) { return floorf(pX); }

    inline float Cross(const glm::vec2& pCurrent, const glm::vec2& pOther)
    {
        return pCurrent.x * pOther.y - pCurrent.y * pOther.x;
    }

   inline float Lerp(const float start, const float end, const float interp) {
        return start + (end - start) * interp;
    }
}
