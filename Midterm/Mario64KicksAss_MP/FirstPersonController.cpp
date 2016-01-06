#include "FirstPersonController.h"

#include <iostream>
#include <cmath>

FirstPersonController::FirstPersonController(Hero* hero, float verticalOffset, RotationMode mode):
    hero(hero),
    verticalOffset(verticalOffset),
    rotationMode(mode)
{
}

void FirstPersonController::look()
{
    float theta = hero->getTheta() * M_PI / 180.0;
    float xAngle = hero->getXAngle() * M_PI / 180.0;
    float zAngle = hero->getZAngle() * M_PI / 180.0;

    if (rotationMode == RotationMode::CURVE)
    {
        Point up(-sin(zAngle), cos(zAngle) * cos(xAngle), 0);
        up.normalize();

        Point direction = up.cross(Point(sin(theta), 0, cos(theta) * cos(xAngle)));
        direction.normalize();

        Point position = *(hero->getPositionPointer()) + up * verticalOffset;

        camera.setPosition(position);
        camera.setUp(up);
        camera.setLookAt(position + direction);
    }
    else if (rotationMode == RotationMode::SURFACE)
    {
        Point up(-sin(zAngle), cos(zAngle) * cos(xAngle), 0);
        up.normalize();
        camera.setUp(up);

        Point direction = Point(0, 0, -1).cross(up);
        Point position = *(hero->getPositionPointer()) + up * verticalOffset;

        camera.setPosition(position);

        camera.setLookAt(position + direction);
    }

    camera.look();
}
