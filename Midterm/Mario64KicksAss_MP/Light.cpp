#include "Light.h"

#include "Color.h"

#include <stdexcept>
#include <algorithm>
#include <memory>

std::map<GLenum, std::shared_ptr<Light>> Light::lights;

Light::Light(GLenum lightId):
    lightId(lightId)
{
}

void Light::clearLights()
{
    lights.clear();
}

void Light::removeLight(GLenum light)
{
    throwIfInvalidLight(light);
    lights.erase(light);
}

std::shared_ptr<Light> Light::getLight(GLenum light)
{
    throwIfInvalidLight(light);
    
    // If the specified light does not exist yet, make a new one.
    if (lights.find(light) == lights.end())
    {
        lights[light] = std::make_shared<Light>(light);
    }

    return lights[light];
}

void Light::enable()
{
    enabled = true;
    updateEnabled();
}

void Light::disable()
{
    enabled = false;
    updateEnabled();
}

void Light::toggle()
{
    enabled = !enabled;
    updateEnabled();
}

bool Light::isEnabled() const
{
    return enabled;
}

void Light::makePointLight()
{
    glLightf(lightId, GL_SPOT_EXPONENT, 0);
    glLightf(lightId, GL_SPOT_CUTOFF, 180);
}

void Light::makeSpotLight(GLfloat exponent,
                          GLfloat cutoff)
{
    glLightf(lightId, GL_SPOT_EXPONENT, exponent);
    glLightf(lightId, GL_SPOT_CUTOFF, cutoff);
}

void Light::setPosition(const Point& point)
{
    std::array<GLfloat, 4> position = {
        point.getX(),
        point.getY(),
        point.getZ(),
        1
    };

    glLightfv(GL_LIGHT0, GL_POSITION, position.data());
}

void Light::setDirection(const Point& point)
{
    std::array<GLfloat, 4> direction = {
        point.getX(),
        point.getY(),
        point.getZ(),
        0
    };

    glLightfv(GL_LIGHT0, GL_POSITION, direction.data());
}

void Light::setSpotDirection(const Point& point)
{
    std::array<GLfloat, 3> direction = {
        point.getX(),
        point.getY(),
        point.getZ(),
    };

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction.data());
}

void Light::setDiffuse(const Color& color)
{
    glLightfv(lightId, GL_DIFFUSE, color.getRgba().data());
}

void Light::setAmbient(const Color& color)
{
    glLightfv(lightId, GL_AMBIENT, color.getRgba().data());
}

void Light::setSpecular(const Color& color)
{
    glLightfv(lightId, GL_SPECULAR, color.getRgba().data());
}

void Light::updateEnabled()
{
    if (enabled)
    {
        glEnable(lightId);
    }
    else 
    {
        glDisable(lightId);
    }
}

void Light::throwIfInvalidLight(GLenum light)
{
    // If the light is not in the allowed range, throw.
    if (light < GL_LIGHT0 || light >= GL_LIGHT0 + NUM_LIGHTS)
    {
        throw std::invalid_argument("Invalid light specified");
    }
}
