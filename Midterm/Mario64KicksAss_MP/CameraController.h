#pragma once

#include "Camera.h"

/**
 * Abstract class for an OpenGL camera controller which takes keyboard and
 * mouse input and moves a camera based on it.
 */
class CameraController
{
public:

    CameraController() = default;

    /**
     * Construct a CameraController controlling the given
     * camera.
     */
    CameraController(const Camera& camera);
    
    /**
     * Accept a mouse click.
     */
    virtual void mouseButton(int button, int state, int x, int y) = 0;

    /**
     * Accept mouse motion.
     */
    virtual void mouseMotion(int x, int y) = 0;

    /**
     * Accept input from the keyboard.
     */
    virtual void keyboardInput(unsigned char key, int x, int y) = 0;

    /**
     * Call gluLookAt with the internal camera
     */
    virtual void look() = 0;

    /**
     * Coefficient to use for rotating based on mouse movement.
     * Could be changed to be dynamic later.
     */
    const float ROTATION_SPEED = 0.005;

    Camera getCamera();

protected:

    Camera camera;

};
