#include "particle.h"

void Particle::applyForce(const glm::vec3& force) {
    acceleration += force / mass;
}

void Particle::integrate(float deltaTime) {
    oldAcceleration = acceleration;
    
    velocity += ((acceleration + oldAcceleration) / 2.0f) * deltaTime * 0.7f;
    position += velocity * deltaTime + 0.5f * oldAcceleration * deltaTime * deltaTime;
}

void Particle::resetForces() {
    oldAcceleration = acceleration;
    acceleration = glm::vec3(0.0f);
}