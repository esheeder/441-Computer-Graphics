#include "ArcballController.h"

#include "common.h"

#include <iostream>
#include <cmath>

ArcballController::ArcballController(std::shared_ptr<Point> lookAt, float radius, float theta, float phi):
    radius(radius),
    theta(theta),
    phi(phi),
    lastX(0),
    lastY(0),
    rotating(false)
{
    camera.setLookAtPointer(lookAt);
    camera.setUp(Point(0, 1, 0));
}


void ArcballController::mouseButton(int button, int state, int x, int y)
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

void ArcballController::mouseMotion(int x, int y)
{
    if (rotating)
    {
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
        {
            float diff = y - lastY;
            radius += diff;
        }
        else
        {
            theta += (x - lastX) * ROTATION_SPEED;
            phi += -(y - lastY) * ROTATION_SPEED;
        }

    }

    phi = max(0.0001f, phi);
    phi = min((float)M_PI - 0.0001f, phi);

    lastX = x;
    lastY = y;
}

void ArcballController::look()
{
    Point newPosition(radius * sin(theta) * sin(phi) + camera.getLookAt().getX(),
                      radius * cos(phi) + camera.getLookAt().getY(),
                      radius * -cos(theta) * sin(phi) + camera.getLookAt().getZ());
    camera.setPosition(newPosition);
    camera.look();
}
