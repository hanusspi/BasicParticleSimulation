#include "core.h"

InstanceData::InstanceData(const glm::vec3& position, const glm::vec3& rotation, 
                          const glm::vec3& scale, const glm::vec3& col)
    : color(col) {
    
    glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    
    // Create rotation matrix from Euler angles (pitch, yaw, roll)
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    
    // Combine transformations: T * R * S
    modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;
}

InstanceData::InstanceData(const glm::vec3& position, const glm::vec3& rotation, 
                          float uniformScale, const glm::vec3& col)
    : InstanceData(position, rotation, glm::vec3(uniformScale), col) {
}

InstanceData::InstanceData(const glm::vec3& pos, float scale, const glm::vec3& col, 
                          const glm::mat3& rot) : color(col) {
    
    glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
    
    // Convert 3x3 rotation matrix to 4x4
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rotationMatrix[i][j] = rot[i][j];
        }
    }
    
    // Combine transformations: T * R * S
    modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;
}