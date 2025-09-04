#include "particle_simulation.h"
#include <iostream>

ParticleSimulation::ParticleSimulation() 
    : particleSystem(std::make_unique<ParticleSystem>()),
      physicsEngine(nullptr),
      geometryGen(std::make_unique<sphere>(20)) {
}

void ParticleSimulation::initialize(const SimulationConfig& cfg) {
    config = cfg;
    physicsEngine = std::make_unique<PhysicsEngine>(config);
    
    particleSystem->initialize(config.numParticles, config.particleDefaults);
    
    initializeGeometry();
    updateRenderData();
    
    std::cout << "Particle simulation initialized with " << config.numParticles << " particles." << std::endl;
}

void ParticleSimulation::step(float deltaTime) {
    physicsEngine->step(*particleSystem, deltaTime);
    updateRenderData();
}

const RenderData& ParticleSimulation::getRenderData() const {
    return renderData;
}

void ParticleSimulation::reset() {
    particleSystem->initialize(config.numParticles, config.particleDefaults);
    updateRenderData();
}

void ParticleSimulation::updateRenderData() {
    const auto& particles = particleSystem->getParticles();
    
    renderData.clear();
    
    // Get sphere render group
    RenderGroup* sphereGroup = renderData.getRenderGroup("sphere");
    if (!sphereGroup) {
        return; // Geometry not initialized yet
    }
    
    sphereGroup->instances.clear();
    sphereGroup->instances.reserve(particles.size());
    
    for (const auto& particle : particles) {
        InstanceData instance(particle.position, particle.radius, particle.color, particle.rotation);
        sphereGroup->instances.push_back(instance);
    }
    
    sphereGroup->instanceCount = sphereGroup->instances.size();
}

void ParticleSimulation::initializeGeometry() {
    geometryGen->createGeometry();
    
    // Convert to GeometryData and add to renderData
    GeometryData sphereGeometry = geometryGen->toGeometryData("sphere");
    renderData.addRenderGroup("sphere", sphereGeometry);
}