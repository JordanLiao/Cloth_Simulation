#include "Triangle.h"

Triangle::Triangle(Particle* part1, Particle* part2, Particle* part3) {
	p1 = part1;
	p2 = part2;
	p3 = part3;
	area = 0.5f * glm::length(glm::cross((p2->position - p1->position),
		(p3->position - p1->position)));
	normal = glm::normalize(glm::cross((p2->position - p1->position),
		(p3->position - p1->position)));
	p1->setNormal(normal);
	p2->setNormal(normal);
	p3->setNormal(normal);
}

void Triangle::computeAero(glm::vec3 wind) {
	if (glm::length(wind) == 0.0f)
		return;
	glm::vec3 vel = (p1->velocity + p2->velocity + p3->velocity) / 3.0f - wind;
	float crossArea = area * glm::dot(glm::normalize(vel), normal);
	glm::vec3 aForce = (float)(-0.5f * 1.225f * glm::length(vel) * glm::length(vel) * 
		1.1 * crossArea) * normal / 3.f;

	//cout << "tri force: " << aForce.x << " " << aForce.y << " " << aForce.z << endl;
	//cout << "aero force " << aForce.z << endl;
	p1->applyForce(aForce);
	p2->applyForce(aForce);
	p3->applyForce(aForce);
}

void Triangle::update() {
	area = 0.5f * glm::length(glm::cross((p2->position - p1->position),
		(p3->position - p1->position)));
	normal = glm::normalize(glm::cross((p2->position - p1->position),
		(p3->position - p1->position)));
	p1->setNormal(normal);
	p2->setNormal(normal);
	p3->setNormal(normal);
}