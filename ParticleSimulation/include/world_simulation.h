#pragma once
#include "simulation.h"
#include "world.h"
#include "particle_system.h"
#include "physics_engine.h"
#include <memory>

class WorldSimulation : public Simulation {
public:
    WorldSimulation();
    ~WorldSimulation() override = default;
    
    void initialize(const SimulationConfig& config) override;
    void step(float deltaTime) override;
    const RenderData& getRenderData() const override;
    void reset() override;
    
private:
    std::unique_ptr<World> world;
    std::unique_ptr<ParticleSystem> particleSystem;
    std::unique_ptr<PhysicsEngine> physicsEngine;
    SimulationConfig config;
    RenderData renderData;
    
    void setupWorld();
    void updateWorldFromParticles();
};