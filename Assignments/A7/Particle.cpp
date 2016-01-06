#include "Particle.h"
#include <math.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif


Particle::Particle() {

}

Particle::Particle(Point p, Vector v) {
	position = p;
	velocity = v;
	spinAngle = 0;
}

void Particle::update() {

	//Move the particle one step in the direction it is traveling
	position += velocity;

	//Update spinAngle for drawing
	spinAngle++;
}

Point Particle::getLocation() {
	return position;
}

void Particle::draw(double x, double y, double z) {
	glPushMatrix(); {
		
		glTranslatef(position.at(0), position.at(1), position.at(2));
		glRotated(angle, x, y, z);
		glutSolidSphere(3, 10, 10);
	} glPopMatrix();
}