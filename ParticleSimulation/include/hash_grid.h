#pragma once

#include <vector>
#include <glm/glm.hpp>

class HashGrid {
public:
	HashGrid() = default;
	~HashGrid() = default;
	void initalize(float searchRadius);
	void addParticles(const std::vector<glm::vec3>& positions);
	void updateParticles(const std::vector<glm::vec3>& positions);
	void findNeighbors();
	size_t getNeighborCount(size_t particleIndex) const;
	const std::vector<size_t>& getNeighbors(size_t particleIndex) const;
private:
	float cellSize = 0.1f;
	float searchRadius = 0.1f;
	size_t numCells;
	size_t particleCount;
	std::vector<glm::vec3> positions;
	std::vector<int> cellStart;
	std::vector<int> cellEntries;
	std::vector<std::vector<size_t>> neighbors; // Neighbors for each particle
	int computeCellIndex(const glm::vec3& position) const;
	glm::ivec3 computeCellCoords(const glm::vec3& position) const;
	size_t hashCoords(const glm::ivec3& coords) const;
};