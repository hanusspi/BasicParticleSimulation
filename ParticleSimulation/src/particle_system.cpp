#include "particle_system.h"
#include <random>
#include <cmath>

void ParticleSystem::initialize(int count, const ParticleConfig& cfg, const BoundingBox& bounds) {
    config = cfg;
    this->bounds = bounds;
    particles.clear();
    particles.reserve(count);
	hashGrid.initalize(cfg.defaultRadius * 1.5);

    generateInitialPositions(count);
}

void ParticleSystem::generateInitialPositions(int count) {
    // Calculate voxel grid dimensions to fit particles within bounds
    glm::vec3 boundsSize = bounds.max - bounds.min;
    float particleDiameter = config.defaultRadius * 2.0f;
    float spacing = particleDiameter * 1.1f; // 10% spacing between particles

    // Calculate grid dimensions based on bounds and particle spacing
    int gridX = static_cast<int>(boundsSize.x / spacing);
    int gridY = static_cast<int>(boundsSize.y / spacing);
    int gridZ = static_cast<int>(boundsSize.z / spacing);

    // Ensure minimum grid size
    gridX = std::max(gridX, 1);
    gridY = std::max(gridY, 1);
    gridZ = std::max(gridZ, 1);

    // If we can't fit all particles, adjust spacing
    int maxParticles = gridX * gridY * gridZ;
    if (count > maxParticles) {
        // Recalculate grid to accommodate all particles
        int cubeSize = static_cast<int>(std::cbrt(count)) + 1;
        gridX = gridY = gridZ = cubeSize;
        spacing = std::min({
            boundsSize.x / gridX,
            boundsSize.y / gridY,
            boundsSize.z / gridZ
        }) * 0.9f; // Leave some margin
    }

    // Calculate starting position (centered in bounds)
    glm::vec3 startPos = bounds.min + glm::vec3(spacing * 0.5f);
    glm::vec3 gridCenter = startPos + glm::vec3(
        (gridX - 1) * spacing * 0.5f,
        (gridY - 1) * spacing * 0.5f,
        (gridZ - 1) * spacing * 0.5f
    );
    glm::vec3 boundsCenter = (bounds.min + bounds.max) * 0.5f;
    glm::vec3 offset = boundsCenter - gridCenter;

    int particlesCreated = 0;
    for (int z = 0; z < gridZ && particlesCreated < count; ++z) {
        for (int y = 0; y < gridY && particlesCreated < count; ++y) {
            for (int x = 0; x < gridX && particlesCreated < count; ++x) {
                glm::vec3 position = startPos + offset + glm::vec3(
                    x * spacing,
                    y * spacing,
                    z * spacing
                );

                // Ensure particle stays within bounds
                position = bounds.clamp(position);

                float radius = config.defaultRadius;

                Particle particle(position, radius, config.defaultMass, generateRandomColor());
                particle.velocity = generateRandomVelocity();

                particles.push_back(particle);
                particlesCreated++;
            }
        }
    }

    // Initialize hash grid with particle positions
    std::vector<glm::vec3> positions;
    for(const auto& p : particles) {
        positions.push_back(p.position);
	}

	hashGrid.addParticles(positions);
}

glm::vec3 ParticleSystem::generateRandomColor() const {
    return glm::vec3(
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX
    );
}

glm::vec3 ParticleSystem::generateRandomVelocity() const {
    return glm::vec3(
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * config.velocityRange.x,
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * config.velocityRange.y,
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * config.velocityRange.z
    );
}

void ParticleSystem::updateParticles(float deltaTime) {
    for (auto& particle : particles) {
        particle.integrate(deltaTime);
    }


}

std::vector<CollisionPair> ParticleSystem::checkCollisions() {
        std::vector<CollisionPair> collisions;
		
		bool naiveCheck = false;
        if (naiveCheck) {

            for (size_t i = 0; i < particles.size(); ++i) {
                for (size_t j = i + 1; j < particles.size(); ++j) {
                    glm::vec3 direction = particles[j].position - particles[i].position;
                    float distance = glm::length(direction);
                    float minDistance = particles[i].radius + particles[j].radius;

                    if (distance < minDistance && distance > 0.001f) {
                        float overlap = minDistance - distance;
                        CollisionPair collision{
                            i, j, overlap, glm::normalize(direction)
                        };
                        collisions.push_back(collision);
                    }
                }
            }
        }
        else
        {

            std::vector<glm::vec3> positions;
            for (const auto& p : particles) {
                positions.push_back(p.position);
            }
            hashGrid.updateParticles(positions);
            hashGrid.findNeighbors();
            for (size_t i = 0; i < particles.size(); ++i) {
                auto neighborCount = hashGrid.getNeighborCount(i);
                Particle& pA = particles[i];
                for (int j = 0; j < neighborCount; ++j) {
                    size_t neighborIndex = hashGrid.getNeighbors(i)[j];

                    // Skip duplicate pairs and self-collision
                    if (neighborIndex <= i) continue;

                    Particle& pB = particles[neighborIndex];
                    glm::vec3 direction = pB.position - pA.position;
                    float distance = glm::length(direction);
                    float minDistance = pA.radius + pB.radius;

                    if (distance < minDistance && distance > 0.001f) {
                        float overlap = minDistance - distance;
                        CollisionPair collision{
                            i, neighborIndex, overlap, glm::normalize(direction)  // ✅ Fixed indices
                        };
                        collisions.push_back(collision);
                    }
                }
            }
        }

    
	//printf("Detected %zu collisions\n", collisions.size());
	//printf("Detected %zu naive collisions\n", naaiveCollisions.size());
    return collisions;
}

void ParticleSystem::reset() {
    for (auto& particle : particles) {
        particle.resetForces();
    }
}