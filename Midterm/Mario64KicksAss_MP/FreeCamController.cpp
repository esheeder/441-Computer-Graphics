#include "FreeCamController.h"

#include "common.h"

#include <cmath>

// TODO: move to class
const float SPEED = 1.5;

FreeCamController::FreeCamController(const Point& position, float theta, float phi):
    position(position),
    theta(theta),
    phi(phi)
{
    camera.setUp(Point(0, 1, 0));
    computeDirection();
    std::shared_ptr<Point> lookAt = std::make_shared<Point>(position + direction);
    camera.setLookAtPointer(lookAt);
    camera.setPosition(position);
}

void FreeCamController::mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            rotating = true;
        }
        else
        {
            rotating = false;
        }
    }
    lastX = x;
    lastY = y;
}

void FreeCamController::mouseMotion(int x, int y)
{
    if (rotating)
    {
        theta += (x - lastX) * ROTATION_SPEED;
        phi += -(y - lastY) * ROTATION_SPEED;
    }

    phi = max(0.00001f, phi);
    phi = min((float)M_PI + 0.00001f, phi);

    lastX = x;
    lastY = y;
}

void FreeCamController::keyboardInput(unsigned char key, int x, int y)
{
    computeDirection();
    if (key == 'w')
    {
        position = position + SPEED * direction;
    }
    else if (key == 's')
    {
        position = position + -SPEED * direction;
    }
}

void FreeCamController::look()
{
    computeDirection();
    camera.setPosition(position);
    camera.setLookAt(position + direction);
    camera.look();
}

void FreeCamController::computeDirection()
{
    direction = Point(sin(theta) * sin(phi),
                      -cos(phi),
                      -cos(theta) * sin(phi));
    direction.normalize();
}
