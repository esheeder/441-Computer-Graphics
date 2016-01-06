#pragma once

#include "Color.h"

class Material
{
public:
    /**
     * Construct a material from the given colors.
     *
     * Default arguments are the colors from the OpenGL docs.
     */
    Material(const Color& ambient = Color(0.2, 0.2, 0.2),
             const Color& diffuse = Color(0.8, 0.8, 0.8),
             const Color& specular = Color(0, 0, 0),
             float shininess = 0);

    /**
     * Applies this material using glMaterial calls.
     */
    void apply();
    
private:
    Color ambient, diffuse, specular, emission;
    float shininess;
};
