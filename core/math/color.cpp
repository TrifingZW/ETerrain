//
// Created by TrifingZW on 25-1-27.
//

#include "color.h"

std::string ToHex(const float val)
{
    int v = static_cast<int>(std::round(val * 255.0f));
    v = CLAMP(v, 0, 255);
    std::string ret;

    for (int i = 0; i < 2; i++)
    {
        char c; // 直接使用 char 类型，无需 char32_t

        if (const int lv = v & 0xF; lv < 10)
            c = static_cast<char>('0' + lv);
        else
            c = static_cast<char>('a' + lv - 10);

        v >>= 4; // 右移4位处理下一个十六进制数字
        ret = std::string(1, c) + ret; // 将字符转换为字符串并拼接到结果前面
    }

    return ret;
}

static int ParseCol4(const std::string& str, const int ofs)
{
    if (const char character = str[ofs]; character >= '0' && character <= '9')
        return character - '0';
    else
    {
        if (character >= 'a' && character <= 'f')
            return character + (10 - 'a');
        if (character >= 'A' && character <= 'F')
            return character + (10 - 'A');
    }
    return -1;
}

static int ParseCol8(const std::string& str, const int ofs)
{
    return ParseCol4(str, ofs) * 16 + ParseCol4(str, ofs + 1);
}

uint32_t Color::RGBA32() const
{
    uint32_t c = static_cast<uint8_t>(round(R * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(G * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(B * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(A * 255.0f));

    return c;
}

uint32_t Color::ARGB32() const
{
    uint32_t c = static_cast<uint8_t>(round(A * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(R * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(G * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(B * 255.0f));

    return c;
}

uint32_t Color::ABGR32() const
{
    uint32_t c = static_cast<uint8_t>(round(A * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(G * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(B * 255.0f));
    c <<= 8;
    c |= static_cast<uint8_t>(round(R * 255.0f));

    return c;
}

uint64_t Color::RGBA64() const
{
    uint64_t c = static_cast<uint16_t>(round(R * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(G * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(B * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(A * 65535.0f));

    return c;
}

uint64_t Color::ARGB64() const
{
    uint64_t c = static_cast<uint16_t>(round(A * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(R * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(G * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(B * 65535.0f));

    return c;
}

uint64_t Color::ABGR64() const
{
    uint64_t c = static_cast<uint16_t>(round(A * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(B * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(G * 65535.0f));
    c <<= 16;
    c |= static_cast<uint16_t>(round(R * 65535.0f));

    return c;
}

std::string Color::Html(const bool alpha) const
{
    std::string txt;
    txt += ToHex(R);
    txt += ToHex(G);
    txt += ToHex(B);
    if (alpha)
        txt += ToHex(A);

    return txt;
}

float Color::GetH() const
{
    float min = MIN(R, G);
    min = MIN(min, B);
    float max = MAX(R, G);
    max = MAX(max, B);

    const float delta = max - min;

    if (delta == 0.0f)
        return 0.0f;

    float h;
    if (R == max)
        h = (G - B) / delta; // between yellow & magenta
    else if (G == max)
        h = 2 + (B - R) / delta; // between cyan & yellow
    else
        h = 4 + (R - G) / delta; // between magenta & cyan

    h /= 6.0f;
    if (h < 0.0f)
        h += 1.0f;

    return h;
}

float Color::GetS() const
{
    float min = MIN(R, G);
    min = MIN(min, B);
    float max = MAX(R, G);
    max = MAX(max, B);

    const float delta = max - min;

    return max != 0.0f ? delta / max : 0.0f;
}

float Color::GetV() const
{
    float max = MAX(R, G);
    max = MAX(max, B);
    return max;
}

void Color::SetHSV(float h, const float s, const float v, const float alpha)
{
    A = alpha;

    if (s == 0.0f)
    {
        // Achromatic (gray)e
        R = G = B = v;
        return;
    }

    h *= 6.0f;
    h = std::fmodf(h, 6);
    const int i = std::floor(h);

    const float f = h - static_cast<float>(i);
    const float p = v * (1.0f - s);
    const float q = v * (1.0f - s * f);
    const float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
        case 0: // Red is the dominant color
            R = v;
            G = t;
            B = p;
            break;
        case 1: // Green is the dominant color
            R = q;
            G = v;
            B = p;
            break;
        case 2:
            R = p;
            G = v;
            B = t;
            break;
        case 3: // Blue is the dominant color
            R = p;
            G = q;
            B = v;
            break;
        case 4:
            R = t;
            G = p;
            B = v;
            break;
        default: // (5) Red is the dominant color
            R = v;
            G = p;
            B = q;
            break;
    }
}

bool Color::IsEqualApprox(const Color& color) const
{
    return Math::IsEqualApprox(R, color.R) &&
           Math::IsEqualApprox(G, color.G) &&
           Math::IsEqualApprox(B, color.B) &&
           Math::IsEqualApprox(A, color.A);
}

Color Color::Clamp(const Color& min, const Color& max) const
{
    return {
        CLAMP(R, min.R, max.R),
        CLAMP(G, min.G, max.G),
        CLAMP(B, min.B, max.B),
        CLAMP(A, min.A, max.A)
    };
}

void Color::Invert()
{
    R = 1.0f - R;
    G = 1.0f - G;
    B = 1.0f - B;
}

Color Color::Inverted() const
{
    Color c = *this;
    c.Invert();
    return c;
}

Color Color::Hex(uint32_t hex)
{
    const float a = static_cast<float>(hex & 0xFF) / 255.0f;
    hex >>= 8;
    const float b = static_cast<float>(hex & 0xFF) / 255.0f;
    hex >>= 8;
    const float g = static_cast<float>(hex & 0xFF) / 255.0f;
    hex >>= 8;
    const float r = static_cast<float>(hex & 0xFF) / 255.0f;

    return {r, g, b, a};
}

Color Color::Hex64(uint64_t hex)
{
    const float a = static_cast<float>(hex & 0xFFFF) / 65535.0f;
    hex >>= 16;
    const float b = static_cast<float>(hex & 0xFFFF) / 65535.0f;
    hex >>= 16;
    const float g = static_cast<float>(hex & 0xFFFF) / 65535.0f;
    hex >>= 16;
    const float r = static_cast<float>(hex & 0xFFFF) / 65535.0f;

    return {r, g, b, a};
}

Color Color::Html(const std::string& rgba)
{
    std::string color = rgba;
    if (color.empty())
        return {};
    if (color[0] == '#')
        color = color.substr(1);

    const bool is_shorthand = color.length() < 5;
    bool alpha = false;

    if (color.length() == 8 || color.length() == 4)
        alpha = true;
    else if (color.length() == 6 || color.length() == 3)
        alpha = false;

    float r, g, b, a = 1.0f;
    if (is_shorthand)
    {
        r = static_cast<float>(ParseCol4(color, 0)) / 15.0f;
        g = static_cast<float>(ParseCol4(color, 1)) / 15.0f;
        b = static_cast<float>(ParseCol4(color, 2)) / 15.0f;
        if (alpha)
            a = static_cast<float>(ParseCol4(color, 3)) / 15.0f;
    }
    else
    {
        r = static_cast<float>(ParseCol8(color, 0)) / 255.0f;
        g = static_cast<float>(ParseCol8(color, 2)) / 255.0f;
        b = static_cast<float>(ParseCol8(color, 4)) / 255.0f;
        if (alpha)
            a = static_cast<float>(ParseCol8(color, 6)) / 255.0f;
    }

    return {r, g, b, a};
}

bool Color::HtmlIsValid(const std::string& p_color)
{
    std::string color = p_color;

    if (color.empty())
        return false;
    if (color[0] == '#')
        color = color.substr(1);

    // Check if the amount of hex digits is valid.
    const size_t len = color.length();
    if (!(len == 3 || len == 4 || len == 6 || len == 8))
        return false;

    // Check if each hex digit is valid.
    for (int i = 0; i < len; i++)
        if (ParseCol4(color, i) == -1)
            return false;

    return true;
}

Color Color::FromHSV(const float h, const float s, const float v, const float alpha)
{
    Color c;
    c.SetHSV(h, s, v, alpha);
    return c;
}

Color Color::FromRGBE9995(const uint32_t rgbe)
{
    const auto r = static_cast<float>(rgbe & 0x1ff);
    const auto g = static_cast<float>(rgbe >> 9 & 0x1ff);
    const auto b = static_cast<float>(rgbe >> 18 & 0x1ff);
    const auto e = static_cast<float>(rgbe >> 27);
    const float m = std::pow(2.0f, e - 15.0f - 9.0f);

    const float rd = r * m;
    const float gd = g * m;
    const float bd = b * m;

    return {rd, gd, bd, 1.0f};
}

Color Color::operator+(const Color& color) const
{
    return {
        R + color.R,
        G + color.G,
        B + color.B,
        A + color.A
    };
}

void Color::operator+=(const Color& color)
{
    R = R + color.R;
    G = G + color.G;
    B = B + color.B;
    A = A + color.A;
}

Color Color::operator-() const
{
    return {
        1.0f - R,
        1.0f - G,
        1.0f - B,
        1.0f - A
    };
}

Color Color::operator-(const Color& color) const
{
    return {
        R - color.R,
        G - color.G,
        B - color.B,
        A - color.A
    };
}

void Color::operator-=(const Color& color)
{
    R = R - color.R;
    G = G - color.G;
    B = B - color.B;
    A = A - color.A;
}

Color Color::operator*(const Color& color) const
{
    return {
        R * color.R,
        G * color.G,
        B * color.B,
        A * color.A
    };
}

Color Color::operator*(const float scalar) const
{
    return {
        R * scalar,
        G * scalar,
        B * scalar,
        A * scalar
    };
}

void Color::operator*=(const Color& color)
{
    R = R * color.R;
    G = G * color.G;
    B = B * color.B;
    A = A * color.A;
}

void Color::operator*=(const float scalar)
{
    R = R * scalar;
    G = G * scalar;
    B = B * scalar;
    A = A * scalar;
}

Color Color::operator/(const Color& color) const
{
    return {
        R / color.R,
        G / color.G,
        B / color.B,
        A / color.A
    };
}

Color Color::operator/(const float scalar) const
{
    return {
        R / scalar,
        G / scalar,
        B / scalar,
        A / scalar
    };
}

void Color::operator/=(const Color& color)
{
    R = R / color.R;
    G = G / color.G;
    B = B / color.B;
    A = A / color.A;
}

void Color::operator/=(const float scalar)
{
    R = R / scalar;
    G = G / scalar;
    B = B / scalar;
    A = A / scalar;
}
