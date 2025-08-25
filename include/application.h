#pragma once
#include <memory>
#include "renderer.h"
#include "simulation.h"
#include "input_handler.h"
#include "camera.h"
#include "core.h"

class Application {
public:
    Application();
    ~Application() = default;
    
    bool initialize();
    void run();
    void shutdown();
    
private:
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Simulation> simulation;
    std::unique_ptr<InputHandler> inputHandler;
    std::shared_ptr<Camera> camera;
    
    bool running = false;
    bool vsyncEnabled = true;
    
    double lastTime = 0.0;
    double frameTime = 0.0;
    int frameCount = 0;
    
    void mainLoop();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void calculateFrameStats();
    
    SimulationConfig createDefaultConfig() const;
};