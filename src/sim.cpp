#include "sim.h"
#include <iostream>

Sim::Sim() {
	std::cout << "Simulation initialized." << std::endl;
}

void Sim::init(std::vector<glm::vec4>* positions, std::vector<glm::vec3>* colors) {
	Particles.positions.resize(positions->size());
	Particles.velocities.resize(positions->size(), glm::vec3(0.0f));
	Particles.accelerations.resize(positions->size(), glm::vec3(0.0f));
	Particles.colors.resize(colors->size());
	Particles.radii.resize(positions->size(), 0.05);
	Particles.mass.resize(positions->size(), 1.0);
	for (size_t i = 0; i < positions->size(); ++i) {
		Particles.positions[i] = glm::vec3((*positions)[i]);
		Particles.radii[i] = (*positions)[i].w;
		Particles.colors[i] = (*colors)[i];
	}
}

void Sim::update(float deltaTime) {
	for (size_t i = 0; i < Particles.positions.size(); ++i) {
		Particles.accelerations[i] = gravity;
		Particles.velocities[i] += Particles.accelerations[i] * deltaTime;
		Particles.positions[i] += Particles.velocities[i] * deltaTime;
		//if (Particles.positions[i].x < lowerLeft.x || Particles.positions[i].x > upperRight.x ||
		//	Particles.positions[i].y < lowerLeft.y || Particles.positions[i].y > upperRight.y ||
		//	Particles.positions[i].z < lowerLeft.z || Particles.positions[i].z > upperRight.z) {
		//	Particles.positions[i] = glm::clamp(Particles.positions[i], lowerLeft, upperRight);
		//	Particles.velocities[i] *= -0.5f;
		//}
	}
}
