#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aInstancePosScale; // xyz = position, w = scale
layout (location = 3) in vec3 aInstanceColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 ParticleColor;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // Apply instance scale
    vec3 pos = aPos * aInstancePosScale.w;
    
    // Apply instance position
    pos += aInstancePosScale.xyz;
    
    // Calculate world position
    FragPos = pos;
    
    // Transform normal vector
    Normal = aNormal;
    
    // Pass color to fragment shader
    ParticleColor = aInstanceColor;
    
    // Apply projection and view transformations
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
