#ifndef _CLOTH_H_
#define _CLOTH_H_

#include "core.h"
#include <vector> 
#include <iostream>
#include "Particle.h"
#include "SpringDamper.h"
#include "Triangle.h"

using namespace std;

class Cloth {
private:
	GLuint VAO, EBO;
	GLuint VBO[2];
public:
	vector<Particle*> p;
	vector<SpringDamper*> horizontalDampers;
	vector<SpringDamper*> verticalDampers;
	vector<SpringDamper*> cross1Dampers; 
	vector<SpringDamper*> cross2Dampers;
	vector<Triangle*> tri;
	vector<glm::vec3> positions;
	vector<glm::ivec3> surfaces;
	vector<glm::vec3> normals;
	glm::mat4 model;
	glm::vec3 color;
	int dimension;

	Cloth(int x);
	void compute(glm::vec3);
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void move(glm::vec3);
};

#endif