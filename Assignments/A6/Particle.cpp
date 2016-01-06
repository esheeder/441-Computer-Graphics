#include "Particle.h"
#include <math.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif


Particle::Particle() {

}

Particle::Particle(Point p, Vector v, double life) {
	position = p;
	velocity = v;
	lifespan = life;
	age = 0;
}

void Particle::update() {
	//Update age
	age += 1.0 / 60.0;

	//Move the particle one step in the direction it is traveling
	position += velocity;

	//Affect it with gravity
	velocity -= Vector(0.0, 0.03, 0.0);
}

Point Particle::getLocation() {
	return position;
}

void Particle::draw(double angle, double x, double y, double z) {
	glPushMatrix(); {
		
		glTranslatef(position.at(0), position.at(1), position.at(2));
		glRotated(angle, x, y, z);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0); glVertex3f(-1, 1, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -1, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1, -1, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 1, 0);
		glEnd();
	} glPopMatrix();
}

bool Particle::tooOld() {
	if (age >= lifespan) return true;
	return false;
}