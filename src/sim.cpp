#include "sim.h"
#include <iostream>

Sim::Sim() : instancedData(nullptr), instancedColors(nullptr) {
	std::cout << "Simulation initialized." << std::endl;
}

void Sim::init(std::vector<glm::vec4>* positions, std::vector<glm::vec3>* colors) {
	// Store pointers to original data
	instancedData = positions;
	instancedColors = colors;

	// Initialize internal particle data
	Particles.positions.resize(positions->size());
	Particles.velocities.resize(positions->size(), glm::vec3(0.0f));
	Particles.accelerations.resize(positions->size(), glm::vec3(0.0f));
	Particles.forces.resize(positions->size(), glm::vec3(0.0f));
	Particles.oldAccelerations.resize(positions->size(), glm::vec3(0.0f));
	Particles.colors.resize(colors->size());
	Particles.radii.resize(positions->size(), 0.05);
	Particles.mass.resize(positions->size(), 1.0);
	Particles.numParticles = positions->size();

	for (size_t i = 0; i < positions->size(); ++i) {
		Particles.positions[i] = glm::vec3((*positions)[i]);
		Particles.radii[i] = (*positions)[i].w;
		Particles.colors[i] = (*colors)[i];

		// Give particles some initial random velocity
		Particles.velocities[i] = glm::vec3(
			(float(rand()) / RAND_MAX - 0.5f) * 2.0f,
			(float(rand()) / RAND_MAX - 0.5f) * 2.0f,
			(float(rand()) / RAND_MAX - 0.5f) * 2.0f
		);
	}
}

void Sim::update(float deltaTime) {
	// 1. Reset forces for all particles
	for (int i = 0; i < Particles.numParticles; i++) {
		Particles.forces[i] = glm::vec3(0.0f);
		Particles.oldAccelerations[i] = Particles.accelerations[i]; 
	}

	// 2. Calculate collision forces between all particle pairs
	for (int i = 0; i < Particles.numParticles; i++) {
		for (int j = i + 1; j < Particles.numParticles; j++) {
			glm::vec3 direction = Particles.positions[j] - Particles.positions[i];
			float distance = glm::length(direction);
			float minDistance = Particles.radii[i] + Particles.radii[j];

			if (distance < minDistance && distance > 0.001f) { // Avoid division by zero
				float overlap = minDistance - distance;
				float forceMagnitude = springConstant * overlap;
				glm::vec3 force = glm::normalize(direction) * forceMagnitude;

				Particles.forces[i] -= force; // Push particle i away
				Particles.forces[j] += force; // Push particle j away
			}
		}
	}

	// 3. Update all particles
	for (int i = 0; i < Particles.numParticles; i++) {
		// Apply gravity + collision forces
		Particles.accelerations[i] = gravity + (Particles.forces[i] / static_cast<float>(Particles.mass[i]));

		// Update velocity and position
		Particles.velocities[i] += ((Particles.accelerations[i] + Particles.oldAccelerations[i]) / 2.0f) * deltaTime * 0.7f;
		Particles.positions[i] += Particles.velocities[i] * deltaTime + 0.5f * Particles.oldAccelerations[i] * deltaTime * deltaTime;

		// Handle boundary collisions
		if (Particles.positions[i].x < lowerLeft.x || Particles.positions[i].x > upperRight.x) {
			Particles.positions[i].x = glm::clamp(Particles.positions[i].x, lowerLeft.x, upperRight.x);
			Particles.velocities[i].x *= -0.5f;
		}
		if (Particles.positions[i].y < lowerLeft.y || Particles.positions[i].y > upperRight.y) {
			Particles.positions[i].y = glm::clamp(Particles.positions[i].y, lowerLeft.y, upperRight.y);
			Particles.velocities[i].y *= -0.5f;
		}
		if (Particles.positions[i].z < lowerLeft.z || Particles.positions[i].z > upperRight.z) {
			Particles.positions[i].z = glm::clamp(Particles.positions[i].z, lowerLeft.z, upperRight.z);
			Particles.velocities[i].z *= -0.5f;
		}

		// Write back to GPU data
		(*instancedData)[i].x = Particles.positions[i].x;
		(*instancedData)[i].y = Particles.positions[i].y;
		(*instancedData)[i].z = Particles.positions[i].z;
		(*instancedColors)[i] = Particles.colors[i];
	}
}
