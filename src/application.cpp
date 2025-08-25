#include "application.h"
#include "opengl_renderer.h"
#include "particle_simulation.h"
#include <iostream>

Application::Application() 
    : renderer(std::make_unique<OpenGLRenderer>()),
      simulation(std::make_unique<ParticleSimulation>()),
      inputHandler(std::make_unique<InputHandler>()),
      camera(std::make_shared<Camera>()) {
}

bool Application::initialize() {
    if (!renderer->initialize(1200, 900, "Particle Simulation")) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }
    
    inputHandler->initialize(static_cast<GLFWwindow*>(renderer->getWindow()));
    inputHandler->setCamera(camera);
    
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
    inputHandler->processKeyboard(static_cast<GLFWwindow*>(renderer->getWindow()), 
                                 static_cast<float>(frameTime));
    
    if (inputHandler->shouldClose()) {
        running = false;
    }
    
    if (inputHandler->isVSyncToggled()) {
        vsyncEnabled = !vsyncEnabled;
        renderer->setVSync(vsyncEnabled);
        std::cout << "VSync: " << (vsyncEnabled ? "ON" : "OFF") << std::endl;
        inputHandler->resetVSyncToggle();
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
    inputHandler.reset();
    camera.reset();
    renderer->shutdown();
    renderer.reset();
    
    running = false;
}

SimulationConfig Application::createDefaultConfig() const {
    SimulationConfig config;
    config.numParticles = 2000;
    config.gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    config.springConstant = 10000.0f;
    config.bounds = BoundingBox(glm::vec3(-1.0f), glm::vec3(1.0f));
    config.particleDefaults.defaultRadius = 0.05f;
    config.particleDefaults.defaultMass = 1.0f;
    config.particleDefaults.velocityRange = glm::vec3(2.0f);
    
    return config;
}