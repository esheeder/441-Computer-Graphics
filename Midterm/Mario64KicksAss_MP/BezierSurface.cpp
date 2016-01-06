#include "BezierSurface.h"

//Default Constructor
BezierSurface::BezierSurface() {

}

//Constructor when passed 16 points and resolution values for u, v
BezierSurface::BezierSurface(Point somePoints[16], float someu, float somev) {
	for (int i = 0; i < 16; i++) {
		points[i] = somePoints[i];
	}
	u = someu;
	v = somev;
	//Initialize our 2d array with empty rows
	for (int i = 0; i <= v; i++) {
		vector<Point> row;
		calculatedPoints.push_back(row);
	}
	//Load our 2d array with data points for our surface
	calculate();
}

Point BezierSurface::getPoint(int i) {
	return points[i];
}

Point BezierSurface::calculateCurvePoint(Point p0, Point p1, Point p2, Point p3, float t) {
	Point thePoint = (1 - t)*(1 - t)*(1 - t)*p0 + 3 * (1 - t)*(1 - t)*t*p1 + 3 * (1 - t)*t*t*p2 + t*t*t*p3;
	return thePoint;
}

//This function calcultates all of the points for the surface with the current points, u, and v values
//Stores in 2 dimensional array
void BezierSurface::calculate() {
	for (float i = 0; i <= v; i++) {
		for (float j = 0; j <= u; j++) {
			Point thePoint = (1 - i/v)*(1 - i/v)*(1 - i/v) * calculateCurvePoint(points[0], points[1], points[2], points[3], j/u)
				+ 3 * (1 - i/v)*(1 - i/v)*i/v * calculateCurvePoint(points[4], points[5], points[6], points[7], j/u)
				+ 3 * (1 - i/v)*i/v*i/v * calculateCurvePoint(points[8], points[9], points[10], points[11], j/u)
				+ i/v*i/v*i/v * calculateCurvePoint(points[12], points[13], points[14], points[15], j/u);
			//Store this calculated points in our 2d-array
			calculatedPoints[i].push_back(thePoint);
		}
	}
}

vector< vector< Point> > BezierSurface::getAllPoints() const {
	return calculatedPoints;
}
