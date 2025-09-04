#include "world.h"
#include <glm/gtc/matrix_transform.hpp>

World::World() {
}

void World::initialize() {
    // Pre-cache common geometries
    ensureGeometry("sphere");
    ensureGeometry("cube");
}

void World::update(float deltaTime) {
    // World update logic here (physics, animations, etc.)
    // For now, just placeholder
}

void World::getRenderData(RenderData& renderData) {
    renderData.clear();
    
    // Group objects by geometry type
    std::unordered_map<std::string, std::vector<InstanceData>> groupedInstances;
    
    for (const auto& obj : objects) {
        groupedInstances[obj.geometryType].push_back(obj.instanceData);
    }
    
    // Create render groups
    for (const auto& [geometryType, instances] : groupedInstances) {
        if (geometryCache.find(geometryType) != geometryCache.end()) {
            renderData.addRenderGroup(geometryType, geometryCache[geometryType]);
            
            for (const auto& instance : instances) {
                renderData.addInstance(geometryType, instance);
            }
        }
    }
}

void World::addSphere(const glm::vec3& position, float radius, const glm::vec3& color) {
    ensureGeometry("sphere");
    
    WorldObject obj;
    obj.geometryType = "sphere";
    obj.instanceData = InstanceData(position, glm::vec3(0.0f), radius, color);
    
    objects.push_back(obj);
}

void World::addCube(const glm::vec3& position, const glm::vec3& scale, 
                   const glm::vec3& rotation, const glm::vec3& color) {
    ensureGeometry("cube");
    
    WorldObject obj;
    obj.geometryType = "cube";
    obj.instanceData = InstanceData(position, rotation, scale, color);
    
    objects.push_back(obj);
}

void World::addContainer(const glm::vec3& size, const glm::vec3& position) {
    createContainerWalls(size, position);
}

void World::clear() {
    objects.clear();
}

void World::ensureGeometry(const std::string& type) {
    if (geometryCache.find(type) != geometryCache.end()) {
        return;
    }
    
    if (type == "sphere") {
        sphere sphereGeom(20);
        geometryCache[type] = sphereGeom.toGeometryData(type);
    }
    else if (type == "cube") {
        cube cubeGeom;
        geometryCache[type] = cubeGeom.toGeometryData(type);
    }
    else if (type == "cylinder") {
        cylinder cylinderGeom(1.0f, 2.0f, 20, 1);
        geometryCache[type] = cylinderGeom.toGeometryData(type);
    }
}

void World::createContainerWalls(const glm::vec3& size, const glm::vec3& position) {
    ensureGeometry("cube");
    
    const float wallThickness = 0.1f;
    const glm::vec3 wallColor(0.8f, 0.8f, 0.9f); // Light blue-gray
    
    // Bottom wall
    addCube(position + glm::vec3(0.0f, -size.y - wallThickness, 0.0f),
            glm::vec3(size.x + wallThickness, wallThickness, size.z + wallThickness),
            glm::vec3(0.0f), wallColor);
    
    // Left wall
    addCube(position + glm::vec3(-size.x - wallThickness, 0.0f, 0.0f),
            glm::vec3(wallThickness, size.y, size.z + wallThickness),
            glm::vec3(0.0f), wallColor);
    
    // Right wall
    addCube(position + glm::vec3(size.x + wallThickness, 0.0f, 0.0f),
            glm::vec3(wallThickness, size.y, size.z + wallThickness),
            glm::vec3(0.0f), wallColor);
    
    // Front wall
    addCube(position + glm::vec3(0.0f, 0.0f, -size.z - wallThickness),
            glm::vec3(size.x + wallThickness, size.y, wallThickness),
            glm::vec3(0.0f), wallColor);
    
    // Back wall
    addCube(position + glm::vec3(0.0f, 0.0f, size.z + wallThickness),
            glm::vec3(size.x + wallThickness, size.y, wallThickness),
            glm::vec3(0.0f), wallColor);
}