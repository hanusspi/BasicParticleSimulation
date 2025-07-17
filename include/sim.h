#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


struct particles {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> velocities;
	std::vector<glm::vec3> accelerations;
	std::vector<glm::vec3> colors;
	std::vector<double> radii;
	std::vector<double> mass;

};

class Sim {
public:
	Sim();
	~Sim() = default;
	void init(std::vector<glm::vec4>* positions, std::vector<glm::vec3>* colors);
	void update(float deltaTime);
private:
	particles Particles;
	glm::vec3 gravity = { 0.0, 0.0, -9.81 };
	glm::vec3 lowerLeft = { -1.0, -1.0, -1.0 };
	glm::vec3 upperRight = { 1.0, 1.0, 1.0 };
};