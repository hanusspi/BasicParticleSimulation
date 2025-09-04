#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aInstanceModelMatrix0; // First row of model matrix
layout (location = 3) in vec4 aInstanceModelMatrix1; // Second row of model matrix
layout (location = 4) in vec4 aInstanceModelMatrix2; // Third row of model matrix
layout (location = 5) in vec4 aInstanceModelMatrix3; // Fourth row of model matrix
layout (location = 6) in vec3 aInstanceColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 ParticleColor;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // Reconstruct model matrix from instance data
    mat4 modelMatrix = mat4(
        aInstanceModelMatrix0,
        aInstanceModelMatrix1,
        aInstanceModelMatrix2,
        aInstanceModelMatrix3
    );
    
    // Apply model transformation
    vec4 worldPos = modelMatrix * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // Transform normal vector (use transpose of inverse for proper normal transformation)
    // For uniform scaling and rotation, the upper 3x3 of the model matrix works fine
    mat3 normalMatrix = mat3(modelMatrix);
    Normal = normalize(normalMatrix * aNormal);
    
    // Pass color to fragment shader
    ParticleColor = aInstanceColor;
    
    // Apply projection and view transformations
    gl_Position = projection * view * worldPos;
}
