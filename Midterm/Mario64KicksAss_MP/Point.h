#pragma once

#include <iostream>

using namespace std;

class Point {
private:
	float x, y, z;

public:
	//Constructors
	Point(); //Default constructor
	Point(float a, float b, float c);

	//Getters and setters
	float getX() const;
	float getY() const;
	float getZ() const;
	void setX(float a);
	void setY(float b);
	void setZ(float c);

    Point cross(const Point& other);

    float getMagnitude();
    void normalize();

};

//Overloaded operators
Point operator*(Point p, float f); //multiplies a Point by a float
Point operator*(float f, Point p); //multiplies a float by a Point
Point operator+(Point a, Point b); //adds two Points together
Point operator-(Point a, Point b);

std::ostream& operator<<(std::ostream& out, const Point& p);

// OpenGL function overloads and forwards.
void glVertex(const Point& point);
void glNormal(const Point& point);
void glTranslate(const Point& point);
