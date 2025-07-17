#include "geometry.h"
#include <cmath>

cube::cube() {
    createGeometry();
}

void cube::createGeometry() {
    vertices.clear();
    indices.clear();
    normals.clear();

    vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
    vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
    vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
    vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
    vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
    vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
    vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
    vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));

    indices.push_back({ 0, 1, 2 });
    indices.push_back({ 0, 2, 3 });
    indices.push_back({ 4, 5, 6 });
    indices.push_back({ 4, 6, 7 });
    indices.push_back({ 0, 1, 5 });
    indices.push_back({ 0, 5, 4 });
    indices.push_back({ 1, 2, 6 });
    indices.push_back({ 1, 6, 5 });
    indices.push_back({ 2, 3, 7 });
    indices.push_back({ 2, 7, 6 });
    indices.push_back({ 3, 0, 4 });
    indices.push_back({ 3, 4, 7 });

    // Generate normals for each face
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
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
