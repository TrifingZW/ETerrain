//
// Created by TrifingZW on 25-1-27.
//

#pragma once

#include <cstdint>
#include <string>
#include <cmath>

#include "math_funcs.h"
#include "core/typedefs.h"

struct [[nodiscard]] Color
{
    union
    {
        struct
        {
            float R;
            float G;
            float B;
            float A;
        };

        float components[4] = {0, 0, 0, 1.0};
    };

    Color(): R(), G(), B(), A() {}

    /**
     * RGBA construct parameters.
     * Alpha is not optional as otherwise we can't bind the RGB version for scripting.
     */
    Color(const float r, const float g, const float b, const float a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    /**
     * RGB construct parameters.
     */
    Color(const float r, const float g, const float b)
    {
        R = r;
        G = g;
        B = b;
        A = 1.0f;
    }

    /**
     * Construct a Color from another Color, but with the specified alpha value.
     */
    Color(const Color& c, const float a)
    {
        R = c.R;
        G = c.G;
        B = c.B;
        A = a;
    }

    [[nodiscard]] uint32_t RGBA32() const;
    [[nodiscard]] uint32_t ARGB32() const;
    [[nodiscard]] uint32_t ABGR32() const;
    [[nodiscard]] uint64_t RGBA64() const;
    [[nodiscard]] uint64_t ARGB64() const;
    [[nodiscard]] uint64_t ABGR64() const;
    [[nodiscard]] std::string Html(bool alpha = true) const;
    [[nodiscard]] float GetH() const;
    [[nodiscard]] float GetS() const;
    [[nodiscard]] float GetV() const;
    void SetHSV(float h, float s, float v, float alpha = 1.0f);

    [[nodiscard]] bool IsEqualApprox(const Color& color) const;
    Color Clamp(const Color& min = Color(0, 0, 0, 0), const Color& max = Color(1, 1, 1, 1)) const;
    void Invert();
    Color Inverted() const;

    [[nodiscard]] float GetLuminance() const
    {
        return 0.2126f * R + 0.7152f * G + 0.0722f * B;
    }

    Color Lerp(const Color& target, const float interp) const
    {
        Color res = *this;
        res.R = Math::Lerp(res.R, target.R, interp);
        res.G = Math::Lerp(res.G, target.G, interp);
        res.B = Math::Lerp(res.B, target.B, interp);
        res.A = Math::Lerp(res.A, target.A, interp);
        return res;
    }

    Color Darkened(const float amount) const
    {
        Color res = *this;
        res.R = res.R * (1.0f - amount);
        res.G = res.G * (1.0f - amount);
        res.B = res.B * (1.0f - amount);
        return res;
    }

    Color Lightened(const float amount) const
    {
        Color res = *this;
        res.R = res.R + (1.0f - res.R) * amount;
        res.G = res.G + (1.0f - res.G) * amount;
        res.B = res.B + (1.0f - res.B) * amount;
        return res;
    }

    [[nodiscard]] uint32_t RGBE9995() const
    {
        // https://github.com/microsoft/DirectX-Graphics-Samples/blob/v10.0.19041.0/MiniEngine/Core/Color.cpp
        static constexpr auto kMaxVal = static_cast<float>(0x1FF << 7);
        static constexpr auto kMinVal = 1.f / (1 << 16);

        // Clamp RGB to [0, 1.FF*2^16]
        const float _r = CLAMP(R, 0.0f, kMaxVal);
        const float _g = CLAMP(G, 0.0f, kMaxVal);
        const float _b = CLAMP(B, 0.0f, kMaxVal);

        // Compute the maximum channel, no less than 1.0*2^-15
        const float MaxChannel = MAX(MAX(_r, _g), MAX(_b, kMinVal));

        // Take the exponent of the maximum channel (rounding up the 9th bit) and
        // add 15 to it.  When added to the channels, it causes the implicit '1.0'
        // bit and the first 8 mantissa bits to be shifted down to the low 9 bits
        // of the mantissa, rounding the truncated bits.
        union
        {
            float f;
            int32_t i;
        } R, G, B, E;

        E.f = MaxChannel;
        E.i += 0x07804000; // Add 15 to the exponent and 0x4000 to the mantissa
        E.i &= 0x7F800000; // Zero the mantissa

        // This shifts the 9-bit values we need into the lowest bits, rounding as
        // needed. Note that if the channel has a smaller exponent than the max
        // channel, it will shift even more.  This is intentional.
        R.f = _r + E.f;
        G.f = _g + E.f;
        B.f = _b + E.f;

        // Convert the Bias to the correct exponent in the upper 5 bits.
        E.i <<= 4;
        E.i += 0x10000000;

        // Combine the fields. RGB floats have unwanted data in the upper 9
        // bits. Only red needs to mask them off because green and blue shift
        // it out to the left.
        return E.i | (B.i << 18) | (G.i << 9) | (R.i & 511);
    }

    Color Blend(const Color& over) const
    {
        Color res;
        const float sa = 1.0f - over.A;
        res.A = A * sa + over.A;
        if (res.A == 0)
            return {0, 0, 0, 0};

        res.R = (R * A * sa + over.R * over.A) / res.A;
        res.G = (G * A * sa + over.G * over.A) / res.A;
        res.B = (B * A * sa + over.B * over.A) / res.A;
        return res;
    }

    Color SRGBToLinear() const
    {
        return {
            R < 0.04045f ? R * (1.0f / 12.92f) : std::powf((R + 0.055f) * static_cast<float>(1.0 / (1.0 + 0.055)), 2.4f),
            G < 0.04045f ? G * (1.0f / 12.92f) : std::powf((G + 0.055f) * static_cast<float>(1.0 / (1.0 + 0.055)), 2.4f),
            B < 0.04045f ? B * (1.0f / 12.92f) : std::powf((B + 0.055f) * static_cast<float>(1.0 / (1.0 + 0.055)), 2.4f),
            A
        };
    }

    Color LinearToSRGB() const
    {
        return {
            R < 0.0031308f ? 12.92f * R : (1.0f + 0.055f) * std::powf(R, 1.0f / 2.4f) - 0.055f,
            G < 0.0031308f ? 12.92f * G : (1.0f + 0.055f) * std::powf(G, 1.0f / 2.4f) - 0.055f,
            B < 0.0031308f ? 12.92f * B : (1.0f + 0.055f) * std::powf(B, 1.0f / 2.4f) - 0.055f,
            A
        };
    }

    static Color Hex(uint32_t hex);
    static Color Hex64(uint64_t hex);
    static Color Html(const std::string& rgba);
    static bool HtmlIsValid(const std::string& p_color);
    static Color FromHSV(float h, float s, float v, float alpha = 1.0f);
    static Color FromRGBE9995(uint32_t rgbe);

    float& operator[](const int idx) { return components[idx]; }
    const float& operator[](const int idx) const { return components[idx]; }
    bool operator==(const Color& color) const { return R == color.R && G == color.G && B == color.B && A == color.A; }
    bool operator!=(const Color& color) const { return R != color.R || G != color.G || B != color.B || A != color.A; }

    Color operator+(const Color& color) const;
    void operator+=(const Color& color);

    Color operator-() const;
    Color operator-(const Color& color) const;
    void operator-=(const Color& color);

    Color operator*(const Color& color) const;
    Color operator*(float scalar) const;
    void operator*=(const Color& color);
    void operator*=(float scalar);

    Color operator/(const Color& color) const;
    Color operator/(float scalar) const;
    void operator/=(const Color& color);
    void operator/=(float scalar);
};
