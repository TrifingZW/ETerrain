//
// Created by TrifingZW on 25-1-22.
//

#pragma once

// 通过覆盖潜在的系统特定宏为下面的 constexpr 腾出空间
#undef ABS
#undef SIGN
#undef MIN
#undef MAX
#undef CLAMP

// 通用 ABS 函数，对于数学用途，请使用 Math::abs
template<typename T>
constexpr T ABS(T m_v)
{
    return m_v < 0 ? -m_v : m_v;
}

template<typename T>
constexpr T SIGN(const T m_v)
{
    return m_v > 0 ? +1.0f : (m_v < 0 ? -1.0f : 0.0f);
}

template<typename T, typename T2>
constexpr auto MIN(const T m_a, const T2 m_b)
{
    return m_a < m_b ? m_a : m_b;
}

template<typename T, typename T2>
constexpr auto MAX(const T m_a, const T2 m_b)
{
    return m_a > m_b ? m_a : m_b;
}

template<typename T, typename T2, typename T3>
constexpr auto CLAMP(const T m_a, const T2 m_min, const T3 m_max)
{
    return m_a < m_min ? m_min : (m_a > m_max ? m_max : m_a);
}
