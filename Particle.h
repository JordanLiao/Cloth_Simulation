#ifndef _Particle_H_
#define _Particle_H_

#include <iostream>
#include "core.h"

using namespace std;

class Particle {
public:
	glm::vec3 velocity;
	glm::vec3 force;
	glm::vec3 position;
	glm::vec3 normal;
	float mass;
	bool fixed;
	int numNorm;
	glm::vec3 movement;

	Particle(glm::vec3 pos, float mass);
	void fix();
	void applyForce(glm::vec3);
	void applyGravity();
	void integrate();
	void setNormal(glm::vec3);
	glm::vec3 getNormal();
};

#endif