#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct InstanceData {
    glm::mat4 modelMatrix;    // Full 4x4 transformation matrix
    glm::vec3 color;
    
    InstanceData() : modelMatrix(1.0f), color(1.0f) {}
    
    InstanceData(const glm::mat4& matrix, const glm::vec3& col)
        : modelMatrix(matrix), color(col) {}
    
    // Helper constructor for TRS (Translation, Rotation, Scale)
    InstanceData(const glm::vec3& position, const glm::vec3& rotation, 
                 const glm::vec3& scale, const glm::vec3& col);
    
    // Helper constructor with uniform scale
    InstanceData(const glm::vec3& position, const glm::vec3& rotation, 
                 float uniformScale, const glm::vec3& col);
    
    // Legacy constructor for backward compatibility
    InstanceData(const glm::vec3& pos, float scale, const glm::vec3& col, 
                 const glm::mat3& rot = glm::mat3(1.0f));
};

struct GeometryData {
    std::string name;                    // Unique identifier for geometry type
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;      // Add normals for proper lighting
    std::vector<unsigned int> indices;
    
    GeometryData() = default;
    GeometryData(const std::string& geometryName) : name(geometryName) {}
    
    void clear() {
        vertices.clear();
        normals.clear();
        indices.clear();
    }
    
    bool empty() const {
        return vertices.empty() || indices.empty();
    }
};

struct RenderGroup {
    GeometryData geometry;
    std::vector<InstanceData> instances;
    size_t instanceCount = 0;
    
    RenderGroup() = default;
    RenderGroup(const GeometryData& geom) : geometry(geom) {}
    
    void clear() {
        instances.clear();
        instanceCount = 0;
    }
    
    void reserve(size_t count) {
        instances.reserve(count);
    }
    
    void addInstance(const InstanceData& instance) {
        instances.push_back(instance);
        instanceCount = instances.size();
    }
};

struct RenderData {
    std::unordered_map<std::string, RenderGroup> renderGroups;
    
    void clear() {
        renderGroups.clear();
    }
    
    void addRenderGroup(const std::string& name, const GeometryData& geometry) {
        renderGroups[name] = RenderGroup(geometry);
    }
    
    void addInstance(const std::string& geometryName, const InstanceData& instance) {
        auto it = renderGroups.find(geometryName);
        if (it != renderGroups.end()) {
            it->second.addInstance(instance);
        }
    }
    
    RenderGroup* getRenderGroup(const std::string& name) {
        auto it = renderGroups.find(name);
        return (it != renderGroups.end()) ? &it->second : nullptr;
    }
    
    bool empty() const {
        return renderGroups.empty();
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