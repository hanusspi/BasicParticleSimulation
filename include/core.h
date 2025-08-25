#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct InstanceData {
    glm::vec4 positionScale;
    glm::vec3 color;
    
    InstanceData() = default;
    InstanceData(const glm::vec3& pos, float scale, const glm::vec3& col)
        : positionScale(pos.x, pos.y, pos.z, scale), color(col) {}
};

struct GeometryData {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    
    void clear() {
        vertices.clear();
        indices.clear();
    }
};

struct RenderData {
    std::vector<InstanceData> instances;
    GeometryData geometry;
    size_t instanceCount = 0;
    
    void clear() {
        instances.clear();
        geometry.clear();
        instanceCount = 0;
    }
    
    void reserve(size_t count) {
        instances.reserve(count);
        instanceCount = count;
    }
};

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
    
    BoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds)
        : min(minBounds), max(maxBounds) {}
    
    bool contains(const glm::vec3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
    
    glm::vec3 clamp(const glm::vec3& point) const {
        return glm::vec3(
            glm::clamp(point.x, min.x, max.x),
            glm::clamp(point.y, min.y, max.y),
            glm::clamp(point.z, min.z, max.z)
        );
    }
};

struct ParticleConfig {
    float defaultRadius = 0.05f;
    float defaultMass = 1.0f;
    glm::vec3 velocityRange = glm::vec3(2.0f);
    glm::vec3 colorRange = glm::vec3(1.0f);
};

struct SimulationConfig {
    int numParticles = 2000;
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    float springConstant = 10000.0f;
    BoundingBox bounds = BoundingBox(glm::vec3(-1.0f), glm::vec3(1.0f));
    ParticleConfig particleDefaults;
};