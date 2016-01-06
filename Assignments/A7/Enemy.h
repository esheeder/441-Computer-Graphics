#ifndef _ENEMY_H_
#define _ENEMY_H_ 1

#include "Point.h"
#include "Vector.h"

class Enemy {
public:
	// CONSTRUCTORS / DESTRUCTORS
	Enemy();
	Enemy(Point l, Vector d, double r);

	// MISCELLANEOUS
	double radius;
	Point location;
	Vector direction;

	void draw();
	void update(float heroX, float heroY, float heroZ);
	void moveForward();
	void moveBackward();

private:
	double _rotation;
};

#endif