#include "world_simulation.h"
#include <iostream>

WorldSimulation::WorldSimulation()
    : world(std::make_unique<World>()),
      particleSystem(std::make_unique<ParticleSystem>()),
      physicsEngine(nullptr) {
}

void WorldSimulation::initialize(const SimulationConfig& config) {
    this->config = config;
    
    // Initialize physics engine
    physicsEngine = std::make_unique<PhysicsEngine>(config);
    
    // Initialize particle system
    particleSystem->initialize(config.numParticles, config.particleDefaults, config.bounds);
    
    // Initialize world
    world->initialize();
    setupWorld();
    
    // Update render data for first frame
    updateWorldFromParticles();
    world->getRenderData(renderData);
    
    std::cout << "World simulation initialized with " << config.numParticles 
              << " particles and container walls." << std::endl;
}

void WorldSimulation::step(float deltaTime) {
    // Run physics simulation
    physicsEngine->step(*particleSystem, deltaTime);
    
    // Update world with particle positions
    updateWorldFromParticles();
    
    // Get updated render data
    world->getRenderData(renderData);
}

const RenderData& WorldSimulation::getRenderData() const {
    return renderData;
}

void WorldSimulation::reset() {
    // Reset particle system
    particleSystem->initialize(config.numParticles, config.particleDefaults, config.bounds);
    
    // Reset world (clear and recreate)
    world->clear();
    setupWorld();
    updateWorldFromParticles();
    world->getRenderData(renderData);
}

void WorldSimulation::setupWorld() {
    // Re-enable container walls with proper offset from physics bounds
    glm::vec3 boundsSize = config.bounds.max - config.bounds.min;
    glm::vec3 boundsCenter = (config.bounds.max + config.bounds.min) * 0.5f;
    
    // Create container walls matching physics bounds so visuals align with collision
    glm::vec3 containerSize = boundsSize * 0.49f; // Half-extents almost matching physics bounds
    
    world->addContainer(containerSize, boundsCenter);
    
    std::cout << "Created container with size: (" 
              << containerSize.x << ", " << containerSize.y << ", " << containerSize.z 
              << ") at position: (" 
              << boundsCenter.x << ", " << boundsCenter.y << ", " << boundsCenter.z << ")" << std::endl;
}

void WorldSimulation::updateWorldFromParticles() {
    const auto& particles = particleSystem->getParticles();
    
    // Clear all world objects and re-add everything
    // Note: This recreates container walls each frame, but it's simpler for now
    // TODO: Optimize to only update sphere instances
    world->clear();
    
    // Re-add container walls
    glm::vec3 boundsSize = config.bounds.max - config.bounds.min;
    glm::vec3 boundsCenter = (config.bounds.max + config.bounds.min) * 0.5f;
    glm::vec3 containerSize = boundsSize * 0.49f;
    world->addContainer(containerSize, boundsCenter);
    
    // Add spheres for each particle
    for (const auto& particle : particles) {
        world->addSphere(particle.position, particle.radius, particle.color);
    }
    
    // Only log occasionally to reduce spam
    static int frameCount = 0;
    if (frameCount % 120 == 0) { // Log every ~2 seconds at 60fps
        std::cout << "Updated " << particles.size() << " particle spheres and container (frame " << frameCount << ")" << std::endl;
    }
    frameCount++;
}