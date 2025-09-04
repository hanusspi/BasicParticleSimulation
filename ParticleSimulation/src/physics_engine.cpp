#include "physics_engine.h"

PhysicsEngine::PhysicsEngine(const SimulationConfig& config)
    : gravity(config.gravity), springConstant(config.springConstant), bounds(config.bounds) {
}

void PhysicsEngine::step(ParticleSystem& particleSystem, float deltaTime) {
    particleSystem.reset();
    
    applyGravity(particleSystem);
    resolveCollisions(particleSystem);
    
    auto& particles = particleSystem.getParticles();
    for (auto& particle : particles) {
        particle.integrate(deltaTime);
        handleBoundaryCollisions(particle);
    }
}

void PhysicsEngine::applyGravity(ParticleSystem& particleSystem) {
    auto& particles = particleSystem.getParticles();
    for (auto& particle : particles) {
        particle.applyForce(gravity * particle.mass);
    }
}

void PhysicsEngine::resolveCollisions(ParticleSystem& particleSystem) {
    auto collisions = particleSystem.checkCollisions();
    auto& particles = particleSystem.getParticles();
    
    for (const auto& collision : collisions) {
        glm::vec3 force = calculateCollisionForce(collision, 
                                                 particles[collision.particleA], 
                                                 particles[collision.particleB]);
        
        particles[collision.particleA].applyForce(-force);
        particles[collision.particleB].applyForce(force);
    }
}

void PhysicsEngine::handleBoundaryCollisions(Particle& particle) {
    if (particle.position.x < bounds.min.x || particle.position.x > bounds.max.x) {
        particle.position.x = glm::clamp(particle.position.x, bounds.min.x, bounds.max.x);
        particle.velocity.x *= -0.5f;
    }
    if (particle.position.y < bounds.min.y || particle.position.y > bounds.max.y) {
        particle.position.y = glm::clamp(particle.position.y, bounds.min.y, bounds.max.y);
        particle.velocity.y *= -0.5f;
    }
    if (particle.position.z < bounds.min.z || particle.position.z > bounds.max.z) {
        particle.position.z = glm::clamp(particle.position.z, bounds.min.z, bounds.max.z);
        particle.velocity.z *= -0.5f;
    }
}

glm::vec3 PhysicsEngine::calculateCollisionForce(const CollisionPair& collision, 
                                                const Particle& p1, const Particle& p2) {
    float forceMagnitude = springConstant * collision.overlap;
    return collision.direction * forceMagnitude;
}