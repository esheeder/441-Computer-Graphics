#include "Point.h"

#include "common.h"
#include <cmath>

Point::Point() {
	x = 0;
	y = 0;
	z = 0;
}

//Non-default constructor
Point::Point(float a, float b, float c) {
	x = a;
	y = b;
	z = c;
}

//Getters and setters
float Point::getX() const {
	return x;
}

float Point::getY() const {
	return y;
}

float Point::getZ() const {
	return z;
}

void Point::setX(float a) {
	x = a;
}

void Point::setY(float b) {
	y = b;
}

void Point::setZ(float c) {
	z = c;
}

Point Point::cross(const Point& other)
{
    return Point(y * other.z - z * other.y,
                 z * other.x - x * other.z,
                 x * other.y - y * other.x);
}

float Point::getMagnitude()
{
    return sqrt(x * x + y * y + z * z);
}

void Point::normalize() {
    float magnitude = sqrt(x * x + y * y + z * z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

//Overloaded operators
Point operator*(Point p, float f) {
	return Point(p.getX()*f, p.getY() * f, p.getZ() * f);
}

Point operator*(float f, Point p) {
	return Point(p.getX()*f, p.getY() * f, p.getZ() * f);
}

Point operator+(Point a, Point b) {
	return Point(a.getX()+b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

Point operator-(Point a, Point b) {
	return Point(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
    out << "Point(x=" << p.getX() << ", y=" << p.getY() << ", z=" << p.getZ() << ")"; 
    return out;
}

void glVertex(const Point& point)
{
    glVertex3f(point.getX(), point.getY(), point.getZ());
}

void glNormal(const Point& point)
{
    glNormal3f(point.getX(), point.getY(), point.getZ());
}

void glTranslate(const Point& point)
{
    glTranslatef(point.getX(),
                 point.getY(),
                 point.getZ());
}
