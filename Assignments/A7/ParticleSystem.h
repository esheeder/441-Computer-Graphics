#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_ 1

#include "Particle.h"
#include "Point.h"
#include <vector>
#include <map>

class ParticleSystem {
public:

	//Constructors
	ParticleSystem();
	ParticleSystem(Point p, double angle, double minV, double maxV, double minL, double maxL, double sR);

	//Methods
	//void addParticle(Particle p);
	void updateParticles(float cameraX, float cameraY, float cameraZ);
	void drawParticles(float cameraX, float cameraZ);
	std::vector<Particle> particles;

	//Variables
	Point position;
	double coneAngle;
	double minVelocity;
	double maxVelocity;
	double minLife;
	double maxLife;
	double spawnRate;
	double timer;
	std::map<double, int> particleOrder;
};


#endif
