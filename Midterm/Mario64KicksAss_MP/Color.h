#pragma once

#include <array>

class Color
{
public:

    /**
     * Default constructor; defaults to white
     */
    Color();

    /**
     * Construct a color from the given components
     * in the range [0, 1]
     *
     * Sets alpha to 1.0.
     */
    Color(float r, float g, float b);

    /**
     * Construct a color from the given components
     * in the range [0, 1]
     */
    Color(float r, float g, float b, float a);

    /**
     * Get the red value of the color
     */
    float getR() const;

    /**
     * Get the green value of the color
     */
    float getG() const;

    /**
     * Get the blue value of the color
     */
    float getB() const;

    /**
     * Get the alpha value of the color
     */
    float getA() const;

    /**
     * Get a copy of the internal RGBA array.
     */
    std::array<float, 4> getRgba() const;
    
    /**
     * Static functions to generate some common colors.
     */
    static Color red() {return Color(1, 0, 0);}
    static Color green() {return Color(0, 1, 0);}
    static Color blue() {return Color(0, 0, 1);}

private:
    std::array<float, 4> rgba;
};

void glColor(const Color& color);
void glClearColor(const Color& color);
