#pragma once
#include "core.h"
#include "geometry.h"
#include <memory>
#include <unordered_map>

class World {
public:
    World();
    ~World() = default;
    
    void initialize();
    void update(float deltaTime);
    void getRenderData(RenderData& renderData);
    
    void addSphere(const glm::vec3& position, float radius, const glm::vec3& color);
    void addCube(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec3& color);
    void addContainer(const glm::vec3& size, const glm::vec3& position = glm::vec3(0.0f));
    
    void clear();
    
private:
    struct WorldObject {
        InstanceData instanceData;
        std::string geometryType;
    };
    
    std::vector<WorldObject> objects;
    std::unordered_map<std::string, GeometryData> geometryCache;
    
    void ensureGeometry(const std::string& type);
    void createContainerWalls(const glm::vec3& size, const glm::vec3& position);
};