#pragma once
#include <glm/glm.hpp>

class Particle {
public:
    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    glm::vec3 oldAcceleration{0.0f};
    glm::vec3 color{1.0f};
    float radius = 0.05f;
    float mass = 1.0f;
    
    Particle() = default;
    Particle(const glm::vec3& pos, float r, float m, const glm::vec3& col = glm::vec3(1.0f))
        : position(pos), radius(r), mass(m), color(col) {}
    
    void applyForce(const glm::vec3& force);
    void integrate(float deltaTime);
    void resetForces();
};