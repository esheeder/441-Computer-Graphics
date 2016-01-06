#pragma once

#include "Hero.h"

class SirVive : public Hero {
	public:
		SirVive(std::shared_ptr<Point> location, float spinAngle2, float theta2, float xAngle2, float zAngle2);
		void drawHero();
		void drawBody();
		void drawWheelL();
		void drawWheelR();
};
