#pragma once
#include "simulation.h"
#include "particle_system.h"
#include "physics_engine.h"
#include "geometry.h"
#include <memory>

class ParticleSimulation : public Simulation {
public:
    ParticleSimulation();
    ~ParticleSimulation() override = default;
    
    void initialize(const SimulationConfig& config) override;
    void step(float deltaTime) override;
    const RenderData& getRenderData() const override;
    void reset() override;
    
private:
    std::unique_ptr<ParticleSystem> particleSystem;
    std::unique_ptr<PhysicsEngine> physicsEngine;
    std::unique_ptr<geometry> geometryGen;
    SimulationConfig config;
    RenderData renderData;
    
    void updateRenderData();
    void initializeGeometry();
};