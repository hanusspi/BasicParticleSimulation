#pragma once
#include "core.h"

class Simulation {
public:
    virtual ~Simulation() = default;
    
    virtual void initialize(const SimulationConfig& config) = 0;
    virtual void step(float deltaTime) = 0;
    virtual const RenderData& getRenderData() const = 0;
    virtual void reset() = 0;
};