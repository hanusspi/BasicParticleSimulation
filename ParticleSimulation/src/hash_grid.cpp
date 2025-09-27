#include "hash_grid.h"

void HashGrid::initalize(float cellSize) {
	this->cellSize = cellSize;
	searchRadius = cellSize;
}

void HashGrid::addParticles(const std::vector<glm::vec3>& positions) {
	this->positions = positions;
	this->particleCount = positions.size();
	numCells = 2 * particleCount;
	cellStart.resize(numCells+1);
	cellEntries.resize(particleCount);
	neighbors.resize(particleCount);
	//printf("HashGrid initialized with cell size %.3f and %zu cells\n", cellSize, numCells);
}

void HashGrid::updateParticles(const std::vector<glm::vec3>& positions) {
	this->positions = positions;
	neighbors.resize(positions.size());
	particleCount = positions.size();
	cellEntries.resize(particleCount);
	std::fill(cellStart.begin(), cellStart.end(), 0);
	//printf("HashGrid updated with %zu particles\n", particleCount);
}

void HashGrid::findNeighbors() {
	for(size_t i = 0; i < particleCount; ++i) {
		size_t h = hashCoords(computeCellCoords(positions[i]));
		cellStart[h]++;
	}
	int start = 0;
	for(size_t i = 1; i < numCells; ++i) {
		start += cellStart[i];
		cellStart[i] = start;
	}
	cellStart[numCells] = particleCount;
	for (size_t i = 0; i < particleCount; i++) {
		size_t h = hashCoords(computeCellCoords(positions[i]));
		cellStart[h]--;
		cellEntries[cellStart[h]] = i;
	}


	for(size_t i = 0; i < particleCount; ++i) {
		glm::ivec3 cellCoords = computeCellCoords(positions[i]);
		std::vector<size_t> tmpNeighborIndices;
		for (int x = -1; x <= 1; ++x) {
			for (int y = -1; y <= 1; ++y) {
				for (int z = -1; z <= 1; ++z) {
					glm::ivec3 neighborCoords = cellCoords + glm::ivec3(x, y, z);
					size_t neighborCellIndex = hashCoords(neighborCoords);
					size_t startIdx = cellStart[neighborCellIndex];
					size_t endIdx = cellStart[neighborCellIndex + 1];
					for(size_t i = startIdx; i < endIdx; i++) {
						tmpNeighborIndices.push_back(cellEntries[i]);
						}	
					}
				}
			}
	neighbors[i] = tmpNeighborIndices;
	}	
}

size_t HashGrid::getNeighborCount(size_t particleIndex) const {
	if (particleIndex >= neighbors.size()) return 0;
	return neighbors[particleIndex].size();
}

const std::vector<size_t>& HashGrid::getNeighbors(size_t particleIndex) const {
	static const std::vector<size_t> empty;
	if (particleIndex >= neighbors.size()) return empty;
	return neighbors[particleIndex];
}

glm::ivec3 HashGrid::computeCellCoords(const glm::vec3& position) const {
	return glm::ivec3{
		static_cast<int>(position.x / cellSize),
		static_cast<int>(position.y / cellSize),
		static_cast<int>(position.z / cellSize)
	};
}

size_t HashGrid::hashCoords(const glm::ivec3& coords) const {
	size_t h = (coords.x * 73856093) ^ (coords.y * 19349663) ^ (coords.z * 83492791);
	return std::abs(static_cast<int>(h)) % numCells;
}

