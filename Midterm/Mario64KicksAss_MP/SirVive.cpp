#include "SirVive.h"
#include "Material.h"


SirVive::SirVive(std::shared_ptr<Point> location, float spinAngle2, float theta2, float xAngle2, float zAngle2):
    Hero(location, spinAngle2, theta2, xAngle2, zAngle2) {
	name = "SirVive";
}

void SirVive::drawBody()
{
	glPushMatrix();
	glTranslatef(0, 1, 0);
	Material redMaterial(
		Color(0, 0, 0),
		Color(0.5, 0, 0),
		Color(0.7, 0.6, 0.6),
		0.25);
	redMaterial.apply();
	glScalef(3, 1, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 2, 0);
	Material greenMaterial(
		Color(0.0, 0.05, 0.0),
		Color(0.4, 0.5, 0.4),
		Color(0.04, 0.7, 0.04),
		0.078125);
	greenMaterial.apply();
	glScalef(2, 1, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(0, roofPos, 0);
	glTranslatef(0, 2.5, 0);
	glRotatef(-timer, 0, 1, 0);
	Material yellowMaterial(
		Color(0,0,0),
		Color(0.5, 0.5, 0),
		Color(0.6, 0.6, 0.5),
		0.25);
	yellowMaterial.apply();
	glScalef(1, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

}


void SirVive::drawWheelL() {
	glPushMatrix();
	Material bronzeMaterial(
		Color(0.2125, 0.1275, 0.054),
		Color(0.714, 0.4284, 0.18144),
		Color(0.393548, 0.271906, 0.166721),
		0.2);
	bronzeMaterial.apply();
	glRotatef(-timer, 0, 0, 1);
	glutSolidCone(1,1,20,20);
	glPopMatrix();
}

void SirVive::drawWheelR() {
	glPushMatrix();
	Material bronzeMaterial(
		Color(0.2125, 0.1275, 0.054),
		Color(0.714, 0.4284, 0.18144),
		Color(0.393548, 0.271906, 0.166721),
		0.2);
	bronzeMaterial.apply();
	glRotatef(-timer, 0, 0, 1);
	glutSolidCone(1, -1, 20, 20);
	glPopMatrix();
}

void SirVive::drawHero()
{
	glPushMatrix();
	glScalef(3, 3, 3);
	drawBody();
	glPopMatrix();


	glPushMatrix();
	glScalef(3, 3, 3);
	glTranslatef(1, 1, 1);
	drawWheelL();
	glPopMatrix();


	glPushMatrix();
	glScalef(3, 3, 3);
	glTranslatef(1, 1, -1);
	drawWheelR();
	glPopMatrix();

	glPushMatrix();
	glScalef(3, 3, 3);
	glTranslatef(-1, 1, 1);
	drawWheelL();
	glPopMatrix();

	glPushMatrix();
	glScalef(3, 3, 3);
	glTranslatef(-1, 1, -1);
	drawWheelR();
	glPopMatrix();
}
