#pragma once

#include "point.h"

using namespace std;

class BS {
private:
	Point points[16];
	float u, v;
	//2d array containing v-amount of rows, each row has u-amount of points
	vector <vector <Point> > calcultedPoints;

public:
	//Constructors
	BS();
	BS(Point somePoints[16], float someu, float somev);

	Point getPoint(int i);
	Point calculateCurvePoint();
	void calculate();
	void drawSkeleton();
	void draw();
};