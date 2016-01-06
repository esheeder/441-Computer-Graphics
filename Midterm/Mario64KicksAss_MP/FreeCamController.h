#pragma once

#include "Camera.h"
#include "CameraController.h"
#include "Point.h"

class FreeCamController : public CameraController
{
public:

    /**
     * Construct a FreeCamController with an initial theta and phi angle.
     */
    FreeCamController(const Point& position, float theta, float phi);

    /**
     * Decide whether the camera should be rotating based on mouse clicks.
     */
    virtual void mouseButton(int button, int state, int x, int y);

    /**
     * Rotate the camera based on mouse input.
     */
    virtual void mouseMotion(int x, int y);

    /**
     * Move the camera based on keyboard input.
     */
    virtual void keyboardInput(unsigned char key, int x, int y);

    /**
     * Update camera position, and then update the view based on the camera.
     */
    virtual void look();

private:
    
    /**
     * Update the internal direction from theta and phi
     */
    void computeDirection();

    Point direction;
    Point position;

    float theta, phi;
    int lastX, lastY;
    bool rotating;
};
