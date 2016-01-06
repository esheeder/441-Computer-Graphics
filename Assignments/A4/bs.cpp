#include "bs.h"

//Default Constructor
BS::BS() {

}

//Constructor when passed 16 points and resolution values for u, v
BS::BS(Point somePoints[16], float someu, float somev) {
	points = somePoints;
	u = someu;
	v = somev;
	//Initialize our 2d array with empty rows
	for (int i = 0; i < v; i++) {
		vector<Point> row;
		calculatedPoints.push_back(row);
	}
	//Load our 2d array with data points for our surface
	calculate();
}

Point BS::getPoint(int i) {
	return points[i];
}

Point BS::calculateCurvePoint(Point p0, Point p1, Point p2, Point p3, float t) {
	Point thePoint = (1 - t)*(1 - t)*(1 - t)*p0 + 3 * (1 - t)*(1 - t)*t*p1 + 3 * (1 - t)*t*t*p2 + t*t*t*p3;
	return thePoint;
}

//This function calcultates all of the points for the surface with the current points, u, and v values
//Stores in 2 dimensional array
void BS::calculate() {
	for (float i = 0; i < v; i++) {
		for (float j = 0; j < u; j++) {
			Point thePoint = (1 - i)*(1 - i)*(1 - i) * calculateCurvePoint(points[0], points[1], points[2], points[3], j)
				+ 3 * (1 - i)*(1 - i)*i * calculateCurvePoint(points[4], points[5], points[6], points[7], j)
				+ 3 * (1 - i)*i*i * calculateCurvePoint(points[8], points[9], points[10], points[11], j)
				+ i*i*i * calculateCurvePoint(points[12], points[13], points[14], points[15], j);
			//Store this calculated points in our 2d-array
			calculatedPoints[i].push_back(thePoint);
		}
	}
}

//draws just the 16 control points
void BS::drawSkeleton

//draws all the calculated points
void BS::draw() {

}