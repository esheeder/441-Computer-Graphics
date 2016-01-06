#ifndef _PARTICLE_H_
#define _PARTICLE_H_ 1

#include "Point.h"
#include "Vector.h"

class Particle {
public:

	// CONSTRUCTORS
    Particle();
    Particle(Point p, Vector v, double life);

	// Functions
	void update();
	bool tooOld();
	Point getLocation();
	void draw(double angle, double x, double y, double z);

	//Variables
	Point position;
	Vector velocity;
	double lifespan;
	double age;
};


#endif
