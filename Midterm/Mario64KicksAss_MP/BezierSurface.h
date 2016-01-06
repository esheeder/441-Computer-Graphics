#pragma once

#include "Point.h"
#include <vector>
#include <iostream>

using namespace std;

class BezierSurface {
private:
	Point points[16];
	float u, v;
	//2d array containing v+1-amount of rows, each row has u+1-amount of points
	vector <vector <Point> > calculatedPoints;

public:
	//Constructors
	BezierSurface();
	BezierSurface(Point somePoints[16], float someu, float somev);

	Point getPoint(int i);
	Point calculateCurvePoint(Point p0, Point p1, Point p2, Point p3, float t);
	void calculate();
	vector <vector <Point> > getAllPoints() const;
};
