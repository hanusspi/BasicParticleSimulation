#pragma once
#include <vector> // std::vector
#include <array>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4

class geometry {
public:
    geometry() = default;
    virtual ~geometry() = default; // Add virtual destructor

    virtual void createGeometry() = 0;

    std::vector<glm::vec3> getVertices() const { return vertices; }
    std::vector<std::array<int, 3>> getIndices() const { return indices; }

protected:
    std::vector<glm::vec3> vertices;
    std::vector<std::array<int, 3>> indices;
    std::vector<glm::vec3> normals;
};

class cube : public geometry {
public:
    cube();
    void createGeometry() override; // Add override keyword
};

class sphere : public geometry {
public:
    sphere(int samplePoints = 20); // Default parameter
    void createGeometry() override; // Must match base class signature

private:
    int samplePoints; // Store as member variable
};