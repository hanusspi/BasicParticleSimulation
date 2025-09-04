#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 ParticleColor;

out vec4 FragColor;

void main() {
    // Simple lighting (ambient + diffuse)
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 norm = normalize(Normal);
    
    float ambient = 0.3;
    float diffuse = max(dot(norm, lightDir), 0.0) * 0.7;
    
    vec3 result = (ambient + diffuse) * ParticleColor;
    FragColor = vec4(result, 1.0);
}
