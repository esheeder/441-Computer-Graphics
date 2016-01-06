#pragma once

using namespace std;

class Point {
private:
	float x, y, z;

public:
	//Constructors
	Point(); //Default constructor
	Point(float a, float b, float c);

	//Getters and setters
	float getX();
	float getY();
	float getZ();
	void setX(float a);
	void setY(float b);
	void setZ(float c);

};

//Overloaded operators
Point operator*(Point p, float f); //multiplies a Point by a float
Point operator*(float f, Point p); //multiplies a float by a Point
Point operator+(Point a, Point b); //adds two Points together