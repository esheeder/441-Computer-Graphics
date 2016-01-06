#include "Enemy.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

Enemy::Enemy() {
	radius = 0.5;
	location = Point(0, 0, 0);
	direction = Vector(1, 0, 0);
	_rotation = 0;
}

Enemy::Enemy(Point l, Vector d, double r) : radius(r), location(l), direction(d) {
	_rotation = 0;
}

//Changes the enemy's direction to look towards hero
void Enemy::update(float heroX, float heroY, float heroZ) {
	float headingAngle = atan2(direction.at(2), direction.at(0)) * 180 / 3.14159;
	float angleToHero = atan2(heroZ - location.at(2), heroX - location.at(0)) * 180 / 3.14159;
	float toRotate = angleToHero - headingAngle;
	if (toRotate > 180) toRotate -= 360;
	if (toRotate < -180) toRotate += 360;
	//cout << toRotate << endl;
	//cout << "x: " << direction.at(0) << " z: " << direction.at(2) << endl;
	//Update our direction vector
	if (toRotate > 0.01) {
		float newTheta = headingAngle - 1;
		float newX = cos(newTheta * 3.14159 / 180);
		float newZ = sin(newTheta * 3.14159 / 180);
		//cout << "new x: " << newX << " new z: " << newZ << endl;
		direction = Vector(newX, direction.at(1), newZ);
	}
	else if (toRotate < -0.01) {
		float newTheta = headingAngle + 1;
		float newX = cos(newTheta * 3.14159 / 180);
		float newZ = sin(newTheta * 3.14159 / 180);
		//cout << "new x: " << newX << " new z: " << newZ << endl;
		direction = Vector(newX, direction.at(1), newZ);
	}
}

void Enemy::draw() {
	float stacks = 16, slices = 16;
	float thetaStep = 6.28f / stacks;
	float phiStep = 6.28f / slices;

	direction.normalize();
	Vector rotationAxis = cross(direction, Vector(0, 1, 0));

	glPushMatrix(); {
		glTranslatef(location.at(0), location.at(1), location.at(2));
		glTranslatef(0, radius, 0);
		glRotatef(_rotation, rotationAxis.at(0), rotationAxis.at(1), rotationAxis.at(2));
		//glColor3f(_color.getX(), _color.getY(), _color.getZ());

		for (float theta = 0; theta < 6.28; theta += thetaStep) {
			for (float phi = -3.14; phi < 3.14; phi += phiStep) {
				Point p1(radius * cos(theta) * sin(phi),
					radius * cos(phi),
					radius * sin(theta) * sin(phi));
				Point p2(radius * cos(theta + thetaStep) * sin(phi),
					radius * cos(phi),
					radius * sin(theta + thetaStep) * sin(phi));
				Point p3(radius * cos(theta) * sin(phi + phiStep),
					radius * cos(phi + phiStep),
					radius * sin(theta) * sin(phi + phiStep));
				Point p4(radius * cos(theta + thetaStep) * sin(phi + phiStep),
					radius * cos(phi + phiStep),
					radius * sin(theta + thetaStep) * sin(phi + phiStep));

				Vector n1 = p1 - Point(0, 0, 0);
				Vector n2 = p2 - Point(0, 0, 0);
				Vector n3 = p3 - Point(0, 0, 0);
				Vector n4 = p4 - Point(0, 0, 0);

				glBegin(GL_QUADS); {
					glTexCoord2f(theta / 6.28, (phi + 3.14f) / 6.28);
					glNormal3f(n1.at(0), n1.at(1), n1.at(2));
					p1.glVertex();

					glTexCoord2f((theta + thetaStep) / 6.28, (phi + 3.14f) / 6.28);
					glNormal3f(n2.at(0), n2.at(1), n2.at(2));
					p2.glVertex();

					glTexCoord2f((theta + thetaStep) / 6.28, ((phi + phiStep) + 3.14f) / 6.28);
					glNormal3f(n3.at(0), n3.at(1), n3.at(2));
					p4.glVertex();

					glTexCoord2f(theta / 6.28, ((phi + phiStep) + 3.14f) / 6.28);
					glNormal3f(n4.at(0), n4.at(1), n4.at(2));
					p3.glVertex();
				} glEnd();
			}
		}
	}; glPopMatrix();
}

void Enemy::moveForward() {
	location += direction*0.15;
	_rotation -= ((360.0f / 64.0f));
	if (_rotation < 0) {
		_rotation += 360;
	}
}

void Enemy::moveBackward() {
	location -= direction*0.15;
	_rotation += ((360.0f / 64.0f));
	if (_rotation > 360) {
		_rotation -= 360;
	}
}