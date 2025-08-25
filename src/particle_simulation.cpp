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
    
    renderData.instances.clear();
    renderData.instances.reserve(particles.size());
    renderData.instanceCount = particles.size();
    
    for (const auto& particle : particles) {
        InstanceData instance(particle.position, particle.radius, particle.color);
        renderData.instances.push_back(instance);
    }
}

void ParticleSimulation::initializeGeometry() {
    geometryGen->createGeometry();
    
    auto vertices = geometryGen->getVertices();
    auto indices = geometryGen->getIndices();
    
    renderData.geometry.vertices = std::move(vertices);
    
    renderData.geometry.indices.clear();
    for (const auto& index : indices) {
        renderData.geometry.indices.push_back(index[0]);
        renderData.geometry.indices.push_back(index[1]);
        renderData.geometry.indices.push_back(index[2]);
    }
}