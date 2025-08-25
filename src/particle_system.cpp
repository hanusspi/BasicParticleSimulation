#include "particle_system.h"
#include <random>
#include <cmath>

void ParticleSystem::initialize(int count, const ParticleConfig& cfg) {
    config = cfg;
    particles.clear();
    particles.reserve(count);
    
    generateInitialPositions(count);
}

void ParticleSystem::generateInitialPositions(int count) {
    int cubeSize = static_cast<int>(std::cbrt(count)) + 1;
    
    for (int i = 0; i < count; ++i) {
        int x = i % cubeSize;
        int y = (i / cubeSize) % cubeSize;
        int z = i / (cubeSize * cubeSize);
        
        glm::vec3 position{
            x * 2.0f / cubeSize - 1.0f,
            y * 2.0f / cubeSize - 1.0f,
            z * 2.0f / cubeSize - 1.0f
        };
        
        float radius = config.defaultRadius + 
                      (static_cast<float>(rand()) / RAND_MAX) * 0.03f;
        
        Particle particle(position, radius, config.defaultMass, generateRandomColor());
        particle.velocity = generateRandomVelocity();
        
        particles.push_back(particle);
    }
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
    
    return collisions;
}

void ParticleSystem::reset() {
    for (auto& particle : particles) {
        particle.resetForces();
    }
}