#pragma once

#include "common.h"
#include "Point.h"

#include <vector>
#include <memory>

using namespace std;

class Hero {
protected:
    std::shared_ptr<Point> position;
	//spinAngle is the angle of rotation for the wheels, helicopter blades, whatever, used to animate heroes as they move around
	//Theta is degree of rotation around y-axis, i.e. how much the hero has been turned
	//xAngle and zAngle are the angles of the slope the hero is on, i.e. how tilted the car is
	float spinAngle, petX, petY, petZ, theta, xAngle, zAngle;

	string name;

	//The timer is used for idle animations, and increments by 1 every time the program main timer is called
	int timer;

	//This is the vector of points on the Bezier Curve for the pet to travel along
	vector<Point> controlPoints;
	vector<Point> petPath;

public:
	//Constructors
	Hero();
	Hero(std::shared_ptr<Point> position, float spinAngle2, float theta2, float xAngle2, float zAngle2);

	//Functions
    // TODO: make pure virtual?
	virtual void drawHero();
	virtual void drawPet();
	void incrementTimer();
	void addControlPoint(Point somePoint);
	void calculatePetPath(int resolution);
	void changeTheta(float f);
	void move(float a, float b, float c);

	//Getters and setters
	string getName();
	float getX();
	float getY();
	float getZ();
	float getPetX();
	float getPetY();
	float getPetZ();
	float getSpinAngle();
	float getTheta();
	float getXAngle();
	float getZAngle();
	int getTimer();
	void setX(float f);
	void setY(float f);
	void setZ(float f);
	void setPetX(float f);
	void setPetY(float f);
	void setPetZ(float f);
	void setSpinAngle(float f);
	void setTheta(float f);
	void setXAngle(float f);
	void setZAngle(float f);
    
    Point getPosition();
    void setPosition(const Point& position);
    std::shared_ptr<Point> getPositionPointer();
};
