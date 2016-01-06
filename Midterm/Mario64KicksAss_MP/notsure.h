#pragma once

#include "Hero.h"

#include <memory>

class NotSure : public Hero {
public:
	//Constructor
	NotSure(std::shared_ptr<Point> location, float spinAngle2, float theta2, float xAngle2, float zAngle2);

	//Drawing functions for Hero
	void drawWheel();
	void drawWheels();
	void drawHero();
	//Drawing functions for Pet
	void drawSpinningTeapots();
	void drawPet();
};
