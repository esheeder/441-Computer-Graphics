#include "Camera.h"

#include "common.h"

#include <iostream>

Camera::Camera()
{
	lookAt = make_shared<Point>();
}

Camera::Camera(const Point& position, std::shared_ptr<Point> lookAt, const Point& up) :
	position(position),
	lookAt(lookAt),
	up(up)
{
}

Camera::Camera(const Camera& other) :
	position(other.position),
	lookAt(other.lookAt),
	up(other.up)
{
}


void Camera::look() const
{
	gluLookAt(position.getX(),
		position.getY(),
		position.getZ(),
		lookAt->getX(),
		lookAt->getY(),
		lookAt->getZ(),
		up.getX(),
		up.getY(),
		up.getZ());
}

Point Camera::getPosition()
{
	return position;
}

void Camera::setPosition(const Point& position)
{
	this->position = position;
}

Point Camera::getLookAt()
{
	return *lookAt;
}

void Camera::setLookAt(const Point& lookAt)
{
	*(this->lookAt) = lookAt;
}

void Camera::setLookAtPointer(std::shared_ptr<Point> lookAt)
{
	this->lookAt = lookAt;
}

Point Camera::getUp()
{
    return up;
}

void Camera::setUp(const Point& up)
{
	this->up = up;
}

Point Camera::getDirection()
{
    Point direction = *lookAt - position;
    direction.normalize();
    return direction;
}
