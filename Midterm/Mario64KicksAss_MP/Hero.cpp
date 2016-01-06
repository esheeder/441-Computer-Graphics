#include "Hero.h"

//Default constructor
Hero::Hero() {
    position = std::make_shared<Point>(0, 0, 0);
	petX = 0;
	petY = 0;
	petZ = 0;
	theta = 0;
	xAngle = 0;
	zAngle = 0;
	name = "";
	timer = 0;
}

//Constructor with values passed in
Hero::Hero(std::shared_ptr<Point> position, float spinAngle, float theta, float xAngle, float zAngle):
    position(position),
    spinAngle(spinAngle),
    petX(0),
    petY(0),
    petZ(0),
    theta(theta),
    xAngle(xAngle),
    zAngle(zAngle),
    timer(0)
{
}

void Hero::incrementTimer() {
	timer++;
}

void Hero::addControlPoint(Point somePoint) {
	controlPoints.push_back(somePoint);
}

//Expects a vector of 4, 7, 11, etc points, and some resolution for the curve. Computes all of the points on the curve and stores them in petPath
void Hero::calculatePetPath(int resolution) {
	petPath.clear();
	for (unsigned int i = 0; i < controlPoints.size() - 1; i += 3) {
		for (float j = 0; j < resolution; j++) {
			float t = float(j / resolution);
			Point thePoint = (1 - t)*(1 - t)*(1 - t)*controlPoints[i] + 3 * (1 - t)*(1 - t)*t*controlPoints[i + 1] + 3 * (1 - t)*t*t*controlPoints[i + 2] + t*t*t*controlPoints[i + 3];
			petPath.push_back(thePoint);
		}
	}
}

void Hero::changeTheta(float f) {
	theta += f;
}

void Hero::move(float a, float b, float c) {
	position->setX(position->getX() + a);
	position->setY(position->getY() + b);
	position->setZ(position->getZ() + c);
}

//Setters and Getters
string Hero::getName() {
	return name;
}

float Hero::getX() {
	return position->getX();
}

float Hero::getY() {
	return position->getY();
}

float Hero::getZ() {
	return position->getZ();
}

float Hero::getPetX() {
	return petX;
}

float Hero::getPetY() {
	return petY;
}

float Hero::getPetZ() {
	return petZ;
}

float Hero::getTheta() {
	return theta;
}

float Hero::getSpinAngle() {
	return spinAngle;
}

float Hero::getXAngle() {
	return xAngle;
}

float Hero::getZAngle() {
	return zAngle;
}

int Hero::getTimer() {
	return timer;
}

void Hero::setX(float f) {
	position->setX(f);
}

void Hero::setY(float f) {
	position->setY(f);
}

void Hero::setZ(float f) {
	position->setZ(f);
}

void Hero::setPetX(float f) {
	petX = f;
}

void Hero::setPetY(float f) {
	petY = f;
}

void Hero::setPetZ(float f) {
	petZ = f;
}

void Hero::setSpinAngle(float f) {
	spinAngle = f;
}

void Hero::setTheta(float f) {
	theta = f;
}

void Hero::setXAngle(float f) {
	xAngle = f;
}

void Hero::setZAngle(float f) {
	zAngle = f;
}

Point Hero::getPosition()
{
    return *position;
}

void Hero::setPosition(const Point& position)
{
    *(this->position) = position;
}

std::shared_ptr<Point> Hero::getPositionPointer()
{
    return position;
}

//Virtual functions that depend on the hero, should remain empty
void Hero::drawHero() {

}

void Hero::drawPet() {

}
