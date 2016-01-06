#include "point.h"

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
float Point::getX() {
	return x;
}

float Point::getY() {
	return y;
}

float Point::getZ() {
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