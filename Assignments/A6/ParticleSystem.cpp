#include "ParticleSystem.h"
#include <math.h>
#include <iostream>

using namespace std;

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#define PI 3.14159265

ParticleSystem::ParticleSystem() {
	
}

ParticleSystem::ParticleSystem(Point p, double angle, double minV, double maxV, double minL, double maxL, double sR) {
	position = p;
	coneAngle = angle;
	minVelocity = minV / 10.0;
	maxVelocity = maxV / 10.0;
	minLife = minL;
	maxLife = maxL;
	spawnRate = 60.0 / sR; //convert particles/second to # of frames between each particle spawn for calculation purposes
}

void ParticleSystem::updateParticles(float cameraX, float cameraY, float cameraZ) {
	//First check the particles to see which ones needs to be deleted
	for (int i = 0; i < particles.size(); i++) {
		if (particles.at(i).tooOld()) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}

	timer += 1.0;

	//cout << "timer: " << timer << endl;
	//cout << "# particles: " << particles.size() << endl;
	

	//If it's time to create a new particle, make one with randomized velocity and lifespan based on our inputs
	if (timer >= spawnRate) {
		timer -= spawnRate;

		Point startingLocation = position;
		double theta = 360.0 * (double)rand() / RAND_MAX * PI / 180.0;
		double phi = coneAngle * (double)rand() / RAND_MAX * PI / 180.0;
		double speed = minVelocity + (double)rand() / RAND_MAX * (maxVelocity - minVelocity);

		Vector velocity = Vector(speed*cos(theta)*sin(phi), speed*cos(phi), speed*sin(theta)*sin(phi));
		double lifeTime = minLife + (double)rand() / RAND_MAX * (maxLife - minLife);

		Particle newParticle = Particle(startingLocation, velocity, lifeTime);
		particles.push_back(newParticle);
	}

	//Update each particle
	for (unsigned int i = 0; i < particles.size(); i++) {
		particles.at(i).update();
	}

	//Sort the particles for draw order
	Vector view = Vector(cameraX, cameraY, cameraZ);
	view.normalize();
	particleOrder.clear();
	for (int i = 0; i < particles.size(); i++) {
		Point particleLocation = particles.at(i).getLocation();
		Vector CP = Vector(particleLocation.at(0) - cameraX, particleLocation.at(1) - cameraY, particleLocation.at(2) - cameraZ);
		double theta = acos(dot(view, CP) / (view.mag() * CP.mag()));
		double distance = CP.mag() * cos(theta);
		particleOrder.insert(pair<double, int>(-distance, i));
	}
}

void ParticleSystem::drawParticles(float cameraX, float cameraZ) {

	//Calculate angle of rotation for particles to face camera
	Vector camera = Vector(cameraX, 0.0f, cameraZ);
	camera.normalize();
	Vector part = Vector(0, 0, 1);

	float angle = -acos(dot(camera, part)) * 180 / 3.14159;
	Vector rotate = cross(camera, part);
	rotate.normalize();

	glPushMatrix(); {
		glTranslatef(position.at(0), position.at(1), position.at(2));
		//Draw a cone
		glPushMatrix(); {
			glTranslatef(0, -5, 0);
			glRotatef(-90, 1, 0, 0);
			glutSolidCone(2, 5, 10, 10);
		} glPopMatrix();
		glPopMatrix();
	}

	for (unsigned int i = 0; i < particles.size(); i++) {
		particles.at(i).draw(angle, rotate.at(0), rotate.at(1), rotate.at(2));
	}

}