#include "Color.hpp"
#include "Math/Math.hpp"
namespace RealSix
{
    const Color Color::Black(0.0f, 0.0f, 0.0f);
    const Color Color::White(1.0f, 1.0f, 1.0f);
    const Color Color::Red(1.0f, 0.0f, 0.0f);
    const Color Color::Green(0.0f, 1.0f, 0.0f);
    const Color Color::Blue(0.0f, 0.0f, 1.0f);
    const Color Color::Yellow(1.0f, 1.0f, 0.0f);
    const Color Color::LightYellow(1.0f, 1.0f, 0.88f);
    const Color Color::LightBlue(0.68f, 0.85f, 0.9f);
    const Color Color::LightPink(1.0f, 0.71f, 0.76f);
    const Color Color::LightGreen(0.56f, 0.93f, 0.56f);

    Color::Color()
        : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
    {
    }
    Color::Color(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a)
    {
    }

    Color operator+(const Color &left, const Color &right)
    {
        return Color(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
    }

    Color operator+(const Color &left, float value)
    {
        return Color(left.r + value, left.g + value, left.b + value, left.a + value);
    }
    Color operator+(float value, const Color &right)
    {
        return right + value;
    }

    Color operator-(const Color &left, const Color &right)
    {
        return Color(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a);
    }

    Color operator-(const Color &left, float value)
    {
        return Color(left.r - value, left.g - value, left.b - value, left.a - value);
    }
    Color operator-(float value, const Color &right)
    {
        return right - value;
    }

    Color operator*(const Color &left, float value)
    {
        return Color(left.r * value, left.g * value, left.b * value, left.a * value);
    }
    Color operator*(float value, const Color &right)
    {
        return right * value;
    }
    Color operator*(const Color &left, const Color &right)
    {
        return Color(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
    }

    bool operator==(const Color &left, const Color &right)
    {
        return left.r == right.r &&
               left.g == right.g &&
               left.b == right.b &&
               left.a == right.a;
    }

    Color &Color::operator+=(float value)
    {
        *this = *this + value;
        return *this;
    }
    Color &Color::operator+=(const Color &right)
    {
        *this = *this + right;
        return *this;
    }
    Color &Color::operator-=(float value)
    {
        *this = *this - value;
        return *this;
    }
    Color &Color::operator-=(const Color &right)
    {
        *this = *this - right;
        return *this;
    }
    Color &Color::operator*=(float value)
    {
        *this = *this * value;
        return *this;
    }
    Color &Color::operator*=(const Color &right)
    {
        *this = *this + right;
        return *this;
    }
    Color &Color::operator=(const Color &right)
    {
        r = right.r;
        g = right.g;
        b = right.b;
        a = right.a;

        return *this;
    }

    Color Color::ToSRGB(float gamma = 2.2f) const
    {
        return Color(Math::Pow(r, gamma), Math::Pow(g, gamma), Math::Pow(b, gamma), Math::Pow(a, gamma));
    }

}