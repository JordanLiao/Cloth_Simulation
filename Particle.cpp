#include "Particle.h"

Particle::Particle(glm::vec3 pos, float m) {
	position = pos;
	mass = m;
	force = glm::vec3(0);
	velocity = glm::vec3(0);
	fixed = false;
	normal = glm::vec3(0);
	numNorm = 0;
	movement = glm::vec3(0);
}

void Particle::fix() {
	fixed = true;
}

void Particle::applyForce(glm::vec3 f) {
	force += f;
}

void Particle::applyGravity() {
	applyForce(glm::vec3(0.f, -9.8f, 0.f) * mass);
}

void Particle::integrate() {
	if (!fixed) {
		float stepSize = 0.0012;
		glm::vec3 acc = force / mass;
		velocity += acc * stepSize;
		position += velocity * stepSize;
	}

	//move if fixed and movement > 0.0
	if (fixed && glm::length(movement) > 0.01f) {
		glm::vec3 amount = glm::normalize(movement) / 10.f; // try move by 0.1 unit
		if (glm::length(movement) < glm::length(amount))
			amount = movement;
		movement -= amount;
		position += amount;
	}

	//ground collision
	if (position.y < 0.0f) {
		position.y = 0.001;
		velocity.y = fabs(velocity.y / 4.0f); // realistic ground collision 
		velocity.x = velocity.x * 0.8f;
		velocity.z = velocity.z * 0.8f;
	}

	force = glm::vec3(0);
	
}

void Particle::setNormal(glm::vec3 nor) {
	normal += nor;
	numNorm += 1;
}

glm::vec3 Particle::getNormal() {
	glm::vec3 norm = normal / (float)numNorm;
	normal = glm::vec3(0);
	numNorm = 0;
	return glm::normalize(norm);
}
