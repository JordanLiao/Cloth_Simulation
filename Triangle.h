#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <iostream>
#include "core.h"
#include "Particle.h"

using namespace std;

class Triangle {
public:
	Particle* p1;
	Particle* p2;
	Particle* p3;
	glm::vec3 normal;
	float area;
	Triangle(Particle*, Particle*, Particle*);
	void computeAero(glm::vec3);
	void update();
};

#endif