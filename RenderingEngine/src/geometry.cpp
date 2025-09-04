#include "geometry.h"
#include <cmath>

cube::cube() {
    createGeometry();
}

void cube::createGeometry() {
    vertices.clear();
    indices.clear();
    normals.clear();

    // Create cube with proper per-vertex normals
    // Each face needs separate vertices to have correct normals
    
    // Front face (z = 1)
    vertices.push_back(glm::vec3(-1.0f, -1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    vertices.push_back(glm::vec3( 1.0f, -1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    vertices.push_back(glm::vec3( 1.0f,  1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    vertices.push_back(glm::vec3(-1.0f,  1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Back face (z = -1)
    vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    vertices.push_back(glm::vec3(-1.0f,  1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    vertices.push_back(glm::vec3( 1.0f,  1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    vertices.push_back(glm::vec3( 1.0f, -1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    
    // Top face (y = 1)
    vertices.push_back(glm::vec3(-1.0f,  1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    vertices.push_back(glm::vec3(-1.0f,  1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f,  1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f,  1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Bottom face (y = -1)
    vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f, -1.0f, -1.0f)); normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f, -1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    vertices.push_back(glm::vec3(-1.0f, -1.0f,  1.0f)); normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    
    // Right face (x = 1)
    vertices.push_back(glm::vec3( 1.0f, -1.0f, -1.0f)); normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f,  1.0f, -1.0f)); normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f,  1.0f,  1.0f)); normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3( 1.0f, -1.0f,  1.0f)); normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    
    // Left face (x = -1)
    vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(-1.0f, -1.0f,  1.0f)); normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(-1.0f,  1.0f,  1.0f)); normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    vertices.push_back(glm::vec3(-1.0f,  1.0f, -1.0f)); normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    
    // Generate indices for each face (2 triangles per face)
    for (int face = 0; face < 6; face++) {
        int baseIndex = face * 4;
        // First triangle
        indices.push_back({baseIndex, baseIndex + 1, baseIndex + 2});
        // Second triangle
        indices.push_back({baseIndex, baseIndex + 2, baseIndex + 3});
    }
}

// Sphere implementation (fixed)
sphere::sphere(int samplePoints) : samplePoints(samplePoints) {
    createGeometry();
}

void sphere::createGeometry() {
    vertices.clear();
    indices.clear();
    normals.clear();

    const float PI = 3.14159265358979323846f;
    const float RADIUS = 1.0f;

    for (int i = 0; i <= samplePoints; ++i) {
        float theta = i * PI / samplePoints; // polar angle
        for (int j = 0; j <= samplePoints; ++j) {
            float phi = j * 2.0f * PI / samplePoints; // azimuthal angle
            float x = RADIUS * sin(theta) * cos(phi);
            float y = RADIUS * sin(theta) * sin(phi);
            float z = RADIUS * cos(theta);
            glm::vec3 pos(x, y, z);
            vertices.push_back(pos);
            normals.push_back(glm::normalize(pos)); // Sphere normals = normalized position
        }
    }

    for (int i = 0; i < samplePoints; ++i) {
        for (int j = 0; j < samplePoints; ++j) {
            int first = (i * (samplePoints + 1)) + j;
            int second = first + samplePoints + 1;
            indices.push_back({ first, second, first + 1 });
            indices.push_back({ second, second + 1, first + 1 });
        }
    }
}

// Cylinder implementation
cylinder::cylinder(float radius, float length, int radialSegments, int heightSegments)
    : radius(radius), length(length), radialSegments(radialSegments), heightSegments(heightSegments) {
    createGeometry();
}

void cylinder::createGeometry() {
    vertices.clear();
    indices.clear();
    normals.clear();

    const float PI = 3.14159265358979323846f;
    const float halfLength = length * 0.5f;
    
    // Generate vertices for the cylinder sides
    for (int i = 0; i <= heightSegments; ++i) {
        float y = -halfLength + (static_cast<float>(i) / heightSegments) * length;
        
        for (int j = 0; j <= radialSegments; ++j) {
            float angle = (static_cast<float>(j) / radialSegments) * 2.0f * PI;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            
            vertices.push_back(glm::vec3(x, y, z));
            normals.push_back(glm::normalize(glm::vec3(x, 0.0f, z))); // Side normals
        }
    }
    
    // Generate indices for cylinder sides
    for (int i = 0; i < heightSegments; ++i) {
        for (int j = 0; j < radialSegments; ++j) {
            int first = (i * (radialSegments + 1)) + j;
            int second = first + radialSegments + 1;
            
            indices.push_back({ first, second, first + 1 });
            indices.push_back({ second, second + 1, first + 1 });
        }
    }
    
    // Add center vertices for top and bottom caps
    int topCenterIndex = static_cast<int>(vertices.size());
    vertices.push_back(glm::vec3(0.0f, halfLength, 0.0f));   // Top center
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));         // Top normal
    
    int bottomCenterIndex = static_cast<int>(vertices.size());
    vertices.push_back(glm::vec3(0.0f, -halfLength, 0.0f));  // Bottom center
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));        // Bottom normal
    
    // Generate vertices for top and bottom caps
    for (int j = 0; j <= radialSegments; ++j) {
        float angle = (static_cast<float>(j) / radialSegments) * 2.0f * PI;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        
        // Top cap vertices
        vertices.push_back(glm::vec3(x, halfLength, z));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Bottom cap vertices
        vertices.push_back(glm::vec3(x, -halfLength, z));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    }
    
    // Generate indices for caps
    int topCapStart = topCenterIndex + 2;
    int bottomCapStart = topCapStart + radialSegments + 1;
    
    for (int j = 0; j < radialSegments; ++j) {
        // Top cap triangles (clockwise from above)
        indices.push_back({ topCenterIndex, topCapStart + j, topCapStart + j + 1 });
        
        // Bottom cap triangles (counter-clockwise from below)
        indices.push_back({ bottomCenterIndex, bottomCapStart + j + 1, bottomCapStart + j });
    }
}

GeometryData geometry::toGeometryData(const std::string& name) const {
    GeometryData data(name);
    data.vertices = vertices;
    data.normals = normals;
    
    // Convert array<int, 3> indices to unsigned int vector
    data.indices.reserve(indices.size() * 3);
    for (const auto& triangle : indices) {
        data.indices.push_back(static_cast<unsigned int>(triangle[0]));
        data.indices.push_back(static_cast<unsigned int>(triangle[1]));
        data.indices.push_back(static_cast<unsigned int>(triangle[2]));
    }
    
    return data;
}
