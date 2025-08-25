#pragma once
#include <memory>
#include "core.h"
#include "camera.h"

class Renderer {
public:
    virtual ~Renderer() = default;
    
    virtual bool initialize(int width, int height, const char* title) = 0;
    virtual void render(const RenderData& data, const Camera& camera) = 0;
    virtual void shutdown() = 0;
    
    virtual bool shouldClose() const = 0;
    virtual void swapBuffers() = 0;
    virtual void pollEvents() = 0;
    
    virtual void* getWindow() = 0;
    virtual void getFramebufferSize(int* width, int* height) = 0;
    virtual double getTime() = 0;
    virtual void setVSync(bool enabled) = 0;
};