#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle* part1, Particle* part2, float rl, float sk, float dk) {
	p1 = part1; p2 = part2;
	restLength = rl;
	springK = sk; damperK = dk;
	force = glm::vec3(0);
}

void SpringDamper::computeForce() {
	glm::vec3 e = p2->position - p1->position;
	float l = glm::length(e);
	glm::vec3 closingDir = glm::normalize(e);
	float vClose = glm::dot((p1->velocity - p2->velocity), closingDir);
	//float damperVal = (double)damperK * (double)vClose;
	force = closingDir * (-1.0f * springK * (restLength - l) - damperK * vClose);
	//cout << "springdamper " << force.y << endl;
	p1->applyForce(1.0f * force);
	p2->applyForce(-1.0f * force);
}
