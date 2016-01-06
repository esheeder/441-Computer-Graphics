#include "Color.h"

#include "common.h"

Color::Color()
{
    rgba[0] = 1;
    rgba[1] = 1;
    rgba[2] = 1;
    rgba[3] = 1;
}

Color::Color(float r, float g, float b)
{
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = 1;
}

Color::Color(float r, float g, float b, float a)
{
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

float Color::getR() const
{
    return rgba[0];
}

float Color::getG() const
{
    return rgba[1];
}

float Color::getB() const
{
    return rgba[2];
}

float Color::getA() const
{
    return rgba[3];
}

std::array<float, 4> Color::getRgba() const
{
    return rgba;
}

void glColor(const Color& color)
{
    glColor4f(color.getR(),
              color.getG(),
              color.getB(),
              color.getA());
}

void glClearColor(const Color& color)
{
    glClearColor(color.getR(),
                 color.getG(),
                 color.getB(),
                 color.getA());
}
