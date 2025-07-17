#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


struct particles {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> velocities;
	std::vector<glm::vec3> forces;
	std::vector<glm::vec3> accelerations;
	std::vector<glm::vec3> oldAccelerations;
	std::vector<glm::vec3> colors;
	std::vector<float> radii;
	std::vector<float> mass;
	int numParticles;

};

class Sim {
public:
	Sim();
	~Sim() = default;
	void init(std::vector<glm::vec4>* positions, std::vector<glm::vec3>* colors);
	void update(float deltaTime);
private:
	particles Particles;

	float springConstant = 10000.0;

	glm::vec3 gravity = { 0.0, -9.81, 0.0};
	glm::vec3 lowerLeft = { -1.0, -1.0, -1.0 };
	glm::vec3 upperRight = { 1.0, 1.0, 1.0 };

	std::vector<glm::vec4>* instancedData;
	std::vector<glm::vec3>* instancedColors;
};