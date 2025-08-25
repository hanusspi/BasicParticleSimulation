#pragma once
#include "particle_system.h"
#include "core.h"

class PhysicsEngine {
public:
    PhysicsEngine(const SimulationConfig& config);
    ~PhysicsEngine() = default;
    
    void step(ParticleSystem& particleSystem, float deltaTime);
    
    void setGravity(const glm::vec3& gravity) { this->gravity = gravity; }
    void setSpringConstant(float k) { springConstant = k; }
    void setBounds(const BoundingBox& bounds) { this->bounds = bounds; }
    
private:
    glm::vec3 gravity;
    float springConstant;
    BoundingBox bounds;
    
    void applyGravity(ParticleSystem& particleSystem);
    void resolveCollisions(ParticleSystem& particleSystem);
    void handleBoundaryCollisions(Particle& particle);
    glm::vec3 calculateCollisionForce(const CollisionPair& collision, 
                                     const Particle& p1, const Particle& p2);
};