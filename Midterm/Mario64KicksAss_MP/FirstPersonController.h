#pragma once

#include "Camera.h"
#include "CameraController.h"
#include "Point.h"
#include "Hero.h"

/**
 * Rotations are done in different orders for heroes on the track
 * vs the wandering hero on the surface. It should mathematically
 * be possible to generalize this to any order of rotations, but I
 * got confused with all the angles and just coded calculations for
 * the two orders we use.
 */
enum class RotationMode
{
    SURFACE,
    CURVE
};

class FirstPersonController : public CameraController
{
public:
    
    /**
     * Construct a FirstPersonController with a hero and an offset relative
     * to the center of the hero at which to place the camera.
     */
    FirstPersonController(Hero* hero, float verticalOffset, RotationMode mode);

    /**
     * Ignore mouse clicks.
     */
    virtual void mouseButton(int button, int state, int x, int y) {}

    /**
     * Ignore mouse input.
     */
    virtual void mouseMotion(int x, int y) {}

    /**
     * Ignore keyboard input.
     */
    virtual void keyboardInput(unsigned char key, int x, int y) {}

    /**
     * Update camera position, and then update the view based on the camera.
     */
    virtual void look();

private:
    Hero* hero;
    float verticalOffset;
    RotationMode rotationMode;

};
