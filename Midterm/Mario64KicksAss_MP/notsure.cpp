#include "notsure.h"
#include "Material.h"
#include <iostream>

NotSure::NotSure(std::shared_ptr<Point> location, float spinAngle2, float theta2, float xAngle2, float zAngle2):
    Hero(location, spinAngle2, theta2, xAngle2, zAngle2) {
	name = "Not Sure";
}

//Draws a square wheel that rotates
void NotSure::drawWheel() {
	glRotatef(spinAngle, 0, 0, 1);
	glScalef(1, 1, 0.2);
	glutSolidCube(1);
}

//Draws square wheels for the car that rotate as it moves
void NotSure::drawWheels() {
	Material blueMaterial(
		Color(0.0, 0.1, 0.06),
		Color(0.0, 0.50980392, 0.50980392),
		Color(0.50196078, 0.50196078, 0.50196078),
		0.25);
	blueMaterial.apply();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(2, 0, 2);
		drawWheel();
	}
	glPopMatrix();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(-2, 0, 2);
		drawWheel();
	}
	glPopMatrix();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(2, 0, -2);
		drawWheel();
	}
	glPopMatrix();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(-2, 0, -2);
		drawWheel();
	}
	glPopMatrix();
}

//Draws the vehicle for the hero
void NotSure::drawHero() {
	drawWheels();

	//Draw the body
	glPushMatrix(); {
		Material redMaterial(
			Color(0.05, 0.0, 0.0),
			Color(0.5, 0.4, 0.4),
			Color(0.7, 0.04, 0.04),
			0.078125);
		redMaterial.apply();
		glScalef(4, .5, 4);
		glutSolidCube(1);
	}
	glPopMatrix();

	//Draw a badass teapot at the front
	glPushMatrix(); {
		Material rubyMaterial(
			Color(0.1745, 0.01175, 0.01175),
			Color(0.61424, 0.04136, 0.04136),
			Color(0.727811, 0.626959, 0.626959),
			0.6);
		rubyMaterial.apply();
		glTranslatef(1.5, 1, 0);
		glutSolidTeapot(1);
	}
	glPopMatrix();
}

//Draws spinning teapots around the pet
void NotSure::drawSpinningTeapots() {
	//Draw first teapot
	glPushMatrix(); {
		Material greenMaterial(
			Color(0.0, 0.0, 0.0),
			Color(0.1, 0.35, 0.1),
			Color(0.45, 0.55, 0.45),
			0.25);
		greenMaterial.apply();
		glTranslatef(1, 1, 0);
		glRotatef(3.6*timer, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	} glPopMatrix();
	//Draw second teapot
	glPushMatrix(); {
		Material blueMaterial(
			Color(0.0, 0.1, 0.06),
			Color(0.0,0.50980392, 0.50980392),
			Color(0.50196078, 0.50196078, 0.50196078),
			0.25);
		blueMaterial.apply();
		glTranslatef(-1, 1, 0);
		glRotatef(3.6*timer, 1, 0, 0);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	} glPopMatrix();
	//Draw third teapot
	glPushMatrix(); {
		Material tealMaterial(
			Color(0.1, 0.18725,	0.1745),
			Color(0.396, 0.74151, 0.69102),
			Color(0.297254,	0.30829, 0.306678),
			0.1);
		tealMaterial.apply();
		glTranslatef(-1, -1, 0);
		glRotatef(3.6*timer, 1, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	} glPopMatrix();
	//Draw fourth teapot
	glPushMatrix(); {
		Material blueMaterial(
			Color(0.0, 0.1, 0.06),
			Color(0.0, 0.50980392, 0.50980392),
			Color(0.50196078, 0.50196078, 0.50196078),
			0.25);
		blueMaterial.apply();
		glTranslatef(1, -1, 0);
		glRotatef(3.6*timer, 0, 1, 1);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	} glPopMatrix();
}

//Draws the pet floating around the hero
void NotSure::drawPet() {
	glPushMatrix(); {
		//Center the pet on the Curve
		Point petLocation = petPath[timer % petPath.size()];
		glTranslatef(petLocation.getX(), petLocation.getY(), petLocation.getZ());

		//Draw the spinning teapots around him
		drawSpinningTeapots();

		//Draw his body!
		Material redMaterial(
			Color(0.05, 0.0, 0.0),
			Color(0.5, 0.4, 0.7),
			Color(0.7, 0.04, 0.04),
			0.078125);
		redMaterial.apply();
		glutSolidSphere(0.5, 10, 10);
	} glPopMatrix();
}
