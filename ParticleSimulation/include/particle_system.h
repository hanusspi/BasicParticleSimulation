#pragma once
#include <vector>
#include "particle.h"
#include "core.h"
#include "hash_grid.h"

struct CollisionPair {
    size_t particleA;
    size_t particleB;
    float overlap;
    glm::vec3 direction;
};

class ParticleSystem {
public:
    ParticleSystem() = default;
    ~ParticleSystem() = default;
    
    void initialize(int count, const ParticleConfig& cfg, const BoundingBox& bounds);
    void updateParticles(float deltaTime);
    std::vector<CollisionPair> checkCollisions();
    
    const std::vector<Particle>& getParticles() const { return particles; }
    std::vector<Particle>& getParticles() { return particles; }
    size_t getParticleCount() const { return particles.size(); }
    
    void reset();
    
private:
    std::vector<Particle> particles;
    ParticleConfig config;
    BoundingBox bounds = BoundingBox(glm::vec3(-1.0f), glm::vec3(1.0f));

	HashGrid hashGrid;

    void generateInitialPositions(int count);
    glm::vec3 generateRandomColor() const;
    glm::vec3 generateRandomVelocity() const;
};