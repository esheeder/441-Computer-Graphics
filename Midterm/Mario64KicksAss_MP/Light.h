#pragma once

#include "common.h"
#include "Color.h"
#include "Point.h"

#include <memory>
#include <map>

/**
 * A class representing OpenGL lights. Keeps track of a static map
 * of all lights, and allows manipulating of light properties through
 * objects obtained from a static getter.
 */
class Light
{
public:
    
    Light() = delete;

    Light(GLenum lightId);

    /**
     * Clear the static map of lights.
     */
    static void clearLights();

    /**
     * Deletes the specified light from the static map
     *
     * @param light GL_LIGHTi, where 0 <= i <= 7
     */
    void removeLight(GLenum light);

    /**
     * Get a shared_ptr to the Light object corresponding to the given
     * GLenum identifier.
     *
     * @param light GL_LIGHTi, where 0 <= i <= 7
     * @return A shared_ptr to the Light object
     */
    static std::shared_ptr<Light> getLight(GLenum light);

    /**
     * Enable this light
     */
    void enable();

    /**
     * Disable this light
     */
    void disable();

    /**
     * If this light is enabled, disable it.
     * If it is disabled, enabled it.
     */
    void toggle();

    /**
     * Returns true if the light is enabled, or false otherwise.
     */
    bool isEnabled() const;

    /**
     * Turn this light into a point light.
     * Sets exponent to 0 and cutoff to 180.
     */
    void makePointLight();

    /**
     * Turn this light into a spotlight using
     * the given exponent and cutoff.
     */
    void makeSpotLight(GLfloat exponent, GLfloat cutoff);

    /**
     * Set the diffuse color of this light.
     */
    void setDiffuse(const Color& color);

    /**
     * Set the ambient color of this light.
     */
    void setAmbient(const Color& color);

    /**
     * Set the specular color of this light.
     */
    void setSpecular(const Color& color);

    /**
     * Set the position of this light.
     */
    void setPosition(const Point& point);

    /**
     * Set the direction of this light.
     * Turns the light into a directional light, so should not
     * be used for spotlights.
     *
     * @see setSpotDirection
     */
    void setDirection(const Point& point);

    /**
     * Set the spot direction of this light.
     */
    void setSpotDirection(const Point& point);

    /**
     * Restrict to 8 lights to guarantee OpenGL has enough.
     */
    static const int NUM_LIGHTS = 8;
private:
    
    /**
     * Call glEnable or glDisable based on the state of this light.
     */
    void updateEnabled();

    /**
     * Check whether a light is in the allowed range.
     *
     * @throw std::invalid_argument if the light is outside the range.
     */
    static void throwIfInvalidLight(GLenum light);

    // The map of all the lights in the scene.
    static std::map<GLenum, std::shared_ptr<Light>> lights;

    bool enabled;
    GLenum lightId;
};
