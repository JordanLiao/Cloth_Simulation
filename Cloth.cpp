#include "Cloth.h"

const float MIN_DIMENSION = 7.0f;
const float DOWN_SCALE_FACTOR = 50.f;
//const float REST_LENGTH = 0.0001f;
const float SPRING_K = 10.0f;
const float DAMPER_K = 0.1f;

Cloth::Cloth(int x) {
	dimension = x;
	float restL = MIN_DIMENSION / (x-1);
	float restL1 = sqrtf(restL * restL * 2);
	model = glm::mat4(1);
	color = glm::vec3(.5f, .5f, .2f);

	int i;
	int j;
	//x as row that goes up, y as column that goes to the right
	for (i = 0; i < x; i++) {
		for (j = 0; j < x; j++) {
			glm::vec3 pos(-1.0f * (MIN_DIMENSION / 2) + j * restL, 
			4.0f, i * restL);
			Particle* part = new Particle(pos, 0.001);
			if (i == x - 1) // if the top row
				part->fix();
			p.push_back(part);
			
			if (j != 0) //horizontal dampers
				horizontalDampers.push_back(new SpringDamper(p[i * x + j - 1], p[i * x + j], 
					restL, SPRING_K, DAMPER_K));
			if(i != 0) //vertical dampers
				verticalDampers.push_back(new SpringDamper(p[(i - 1) * x + j], p[i * x + j],
					restL, SPRING_K, DAMPER_K));
			if(i != 0 && j != 0) // cross1dampers /
				cross1Dampers.push_back(new SpringDamper(p[(i - 1) * x + j - 1], 
				p[i * x + j], restL1, SPRING_K, DAMPER_K));
			if(i != 0 && j != x-1) // cross2dampers
				cross2Dampers.push_back(new SpringDamper(p[(i - 1) * x + j + 1], 
				p[i * x + j], restL1, SPRING_K, DAMPER_K));

			positions.push_back(part->position);
		}	
	}
	//init all the triangles
	for (i = 0; i < x - 1; i++) {
		for (j = 0; j < x - 1; j++) {
			Triangle* tri1 = new Triangle(p[i * x + j], p[i * x + j + 1], p[(i + 1) * x + j]);
			surfaces.push_back(glm::ivec3(i * x + j, i * x + j + 1, (i + 1) * x + j));
			Triangle* tri2 = new Triangle(p[(i+1) * x + j], p[i * x + j+1], p[(i + 1) * x + j + 1]);
			surfaces.push_back(glm::ivec3((i + 1) * x + j, i * x + j + 1, (i + 1) * x + j + 1));
			tri.push_back(tri1);
			tri.push_back(tri2);
		}
	}

	vector<Particle*>::iterator pIt = p.begin();
	while (pIt != p.end()) {
		normals.push_back((*pIt)->getNormal());
		pIt++;
	}

	cout << "num of tri: " << tri.size() << endl;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * surfaces.size(), surfaces.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Cloth::compute(glm::vec3 wind) {
	vector<Particle*>::iterator pIt = p.begin();
	while (pIt != p.end()) {
		(*pIt)->applyGravity();
		pIt++;
	}
	vector<SpringDamper*>::iterator dIt = horizontalDampers.begin();
	while (dIt != horizontalDampers.end()) {
		(*dIt)->computeForce();
		dIt++;
	}
	dIt = verticalDampers.begin();
	while (dIt != verticalDampers.end()) {
		(*dIt)->computeForce();
		dIt++;
	}
	dIt = cross1Dampers.begin();
	while (dIt != cross1Dampers.end()) {
		(*dIt)->computeForce();
		dIt++;
	}
	dIt = cross2Dampers.begin();
	while (dIt != cross2Dampers.end()) {
		(*dIt)->computeForce();
		dIt++;
	}

	vector<Triangle*>::iterator triIt = tri.begin();
	while (triIt != tri.end()) {
		(*triIt)->computeAero(wind);
		triIt++;
	}

	int tidx = 0;
	//cout << "particle 1 force: "<<p[tidx]->force.x << " " << p[tidx]->force.y << " " << p[tidx]->force.z << endl;

	pIt = p.begin();
	while (pIt != p.end()) {
		(*pIt)->integrate();
		pIt++;
	}

	//update triangle normals and area
	triIt = tri.begin();
	while (triIt != tri.end()) {
		(*triIt)->update();
		triIt++;
	}

	//update positions
	int i;
	for (i = 0; i < positions.size(); i++) {
		positions[i] = p[i]->position;
	}
	//update normals
	for (i = 0; i < normals.size(); i++) {
		normals[i] = p[i]->getNormal();
	}

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Cloth::move(glm::vec3 move) {
	int idx = (dimension - 1) * dimension;
	for (; idx < p.size(); idx++) {
		//cout << "moving cloth" << endl;
		p[idx]->movement += move;
	}
}

void Cloth::draw(const glm::mat4& viewProjMtx, GLuint shader) {
	// actiavte the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(3.0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Draw the points 
	//glDrawArrays(GL_POINTS, 0, positions.size());
	glDrawElements(GL_TRIANGLES, surfaces.size() * 3, GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}