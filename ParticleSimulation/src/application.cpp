#include "application.h"
#include "opengl_renderer.h"
#include "world_simulation.h"
#include <iostream>

Application::Application() 
    : renderer(std::make_unique<OpenGLRenderer>()),
      simulation(std::make_unique<WorldSimulation>()),
      camera(std::make_shared<Camera>()) {
}

bool Application::initialize() {
    if (!renderer->initialize(1200, 900, "Particle World Simulation")) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }
    
    renderer->setCamera(camera);
    
    simulation->initialize(createDefaultConfig());
    
    lastTime = renderer->getTime();
    running = true;
    
    std::cout << "Application initialized successfully" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move camera" << std::endl;
    std::cout << "  QE - Move up/down" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  V - Toggle VSync" << std::endl;
    std::cout << "  R - Reset camera position" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    return true;
}

void Application::run() {
    if (!running) return;
    
    mainLoop();
}

void Application::mainLoop() {
    while (running && !renderer->shouldClose()) {
        double currentTime = renderer->getTime();
        frameTime = currentTime - lastTime;
        lastTime = currentTime;
        frameCount++;
        
        handleEvents();
        update(static_cast<float>(frameTime));
        render();
        
        calculateFrameStats();
        
        renderer->swapBuffers();
        renderer->pollEvents();
    }
}

void Application::handleEvents() {
    renderer->processInput(static_cast<float>(frameTime));
    
    if (renderer->isCloseRequested()) {
        running = false;
    }
    
    if (renderer->isVSyncToggled()) {
        vsyncEnabled = !vsyncEnabled;
        renderer->setVSync(vsyncEnabled);
        std::cout << "VSync: " << (vsyncEnabled ? "ON" : "OFF") << std::endl;
        renderer->resetVSyncToggle();
    }
}

void Application::update(float deltaTime) {
    int width, height;
    renderer->getFramebufferSize(&width, &height);
    camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    
    simulation->step(deltaTime);
}

void Application::render() {
    renderer->render(simulation->getRenderData(), *camera);
}

void Application::calculateFrameStats() {
    static double lastStatsTime = 0.0;
    static const double statsInterval = 1.0;
    
    if (lastTime - lastStatsTime >= statsInterval) {
        double fps = frameCount / (lastTime - lastStatsTime);
        double msPerFrame = (lastTime - lastStatsTime) * 1000.0 / frameCount;
        
        std::cout << "FPS: " << static_cast<int>(fps) << 
                     " | Frame Time: " << msPerFrame << "ms" << std::endl;
        
        frameCount = 0;
        lastStatsTime = lastTime;
    }
}

void Application::shutdown() {
    std::cout << "Shutting down application..." << std::endl;
    
    simulation.reset();
    camera.reset();
    renderer->shutdown();
    renderer.reset();
    
    running = false;
}

SimulationConfig Application::createDefaultConfig() const {
    SimulationConfig config;
    config.numParticles = 28000;
    config.gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    config.springConstant = 1000.0f;
    // Expand bounds significantly to reduce overcrowding
    config.bounds = BoundingBox(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(5.0f, 5.0f, 5.0f));
    config.particleDefaults.defaultRadius = 0.05f;
    config.particleDefaults.defaultMass = 1.0f;
    config.particleDefaults.velocityRange = glm::vec3(2.0f);
    
    return config;
}