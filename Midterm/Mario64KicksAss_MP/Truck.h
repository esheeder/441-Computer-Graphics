#pragma once

#include "Hero.h"
#include <math.h>

class Truck : public Hero {
	private:
		GLfloat backLegAngle;
		GLfloat backAngleChange;
		GLfloat frontLegAngle;
		GLfloat frontAngleChange;
		vector<Point> controlPoints;
		vector<Point> arcPoints;
		bool brightsOff;
		bool headlightsOff;
		GLfloat t = 0;
		int pointIndex = 0;

		void drawAxle();
		void drawWheels();
		void drawEar();
		void drawTeeth();
		void drawNose();
		void drawHead();
		void drawBackLeg();
		void drawFrontLeg();
		void createSpiralPoints();
		//void renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution);

	public:
		Truck();
		Truck(std::shared_ptr<Point> location, float spinAngle2, float theta2, float xAngle2, float zAngle2);
		void drawPet();
		void drawHero();
		void toggleHeadlights();
		void toggleBrights();
		bool headlightsOn();
		bool brightsOn();
};
