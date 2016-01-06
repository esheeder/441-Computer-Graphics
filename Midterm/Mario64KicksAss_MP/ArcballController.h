#pragma once

#include "Camera.h"
#include "CameraController.h"

class ArcballController : public CameraController
{
public:

    /**
     * Construct an ArcballController with a point to look at, the radius of the camera,
     * and the initial theta and phi angles (in degrees).
     */
    ArcballController(std::shared_ptr<Point> lookAt, float radius, float theta, float phi);

    /**
     * Decide whether the camera should be rotating based on mouse clicks.
     */
    virtual void mouseButton(int button, int state, int x, int y);

    /**
     * Rotate the camera based on mouse input.
     */
    virtual void mouseMotion(int x, int y);

    /**
     * Do nothing on keyboard input.
     */
    virtual void keyboardInput(unsigned char key, int x, int y) {}

    /**
     * Update camera position, and then update the view based on the camera.
     */
    virtual void look();

private:

    /**
     * Private helper to initialize the camera in the constructor.
     */
    static Camera computeCamera(std::shared_ptr<Point> lookAt, float radius, float theta, float phi);

    float radius, theta, phi;
    int lastX, lastY;
    bool rotating;

};
