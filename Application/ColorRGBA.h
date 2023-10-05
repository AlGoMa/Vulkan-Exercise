#pragma once
#include <Vector.h>

namespace App::Math
{
    class ColorRGBA
    {
    public:
        ColorRGBA() = default;

        ColorRGBA(float in_red, float in_green, float in_blue, float in_alpha = 1.f)
        {
            color = Vector(in_red, in_green, in_blue, in_alpha);
        }

        ColorRGBA(Vector in_color) : color(in_color)
        {
        }

        const Vector& Color() const { return color; }

        const ColorRGBA& Normalized() {
            color *= 0.004f;
            color.A = 1.f;
            return (*this);
        }

        ColorRGBA& operator *=(float s)
        {
            color *= s;
            return (*this);
        }

        ColorRGBA& operator /=(float s)
        {
            s = 1.0F / s;
            color /= s;
            return (*this);
        }

        ColorRGBA& operator +=(const ColorRGBA& c)
        {
            color += c.color;
            return (*this);
        }

        ColorRGBA& operator -=(const ColorRGBA& c)
        {
            color -= c.color;
            return (*this);
        }

        ColorRGBA& operator *=(const ColorRGBA& c)
        {
            color *= c.color;
            return (*this);
        }

        friend inline ColorRGBA operator *(const ColorRGBA& c, float s)
        {
            return c.color * s;
        }

        friend inline ColorRGBA operator /(const ColorRGBA& c, float s)
        {
            s = 1.0F / s;
            return c.color / s;
        }

        friend inline ColorRGBA operator +(const ColorRGBA& a, const ColorRGBA& b)
        {
            return a.color + b.color;
        }

        friend inline ColorRGBA operator -(const ColorRGBA& a, const ColorRGBA& b)
        {
            return a.color - b.color;
        }

        friend inline ColorRGBA operator *(const ColorRGBA& a, const ColorRGBA& b)
        {
            return a.color * b.color;
        }

    private:
        Vector color;
    };

}