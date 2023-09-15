#ifndef _SPRINGDAMPER_H_
#define _SPRINGDAMPER_H_

#include <vector> 
#include <iostream>
#include "core.h"
#include "Particle.h"

using namespace std;

class SpringDamper {
public:
	float springK, damperK;
	float restLength;
	glm::vec3 force;
	Particle* p1;
	Particle* p2;
	SpringDamper(Particle * part1, Particle * part2, float rl, float sk, float dk);
	void computeForce();
};

#endif