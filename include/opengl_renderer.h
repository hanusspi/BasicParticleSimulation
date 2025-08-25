#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "renderer.h"
#include "shader.h"

struct RenderBuffers {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int instancedVBO = 0;
    unsigned int instancedColorVBO = 0;
    
    void setup();
    void cleanup();
    void updateInstanceData(const std::vector<InstanceData>& instances);
};

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;
    
    bool initialize(int width, int height, const char* title) override;
    void render(const RenderData& data, const Camera& camera) override;
    void shutdown() override;
    
    bool shouldClose() const override;
    void swapBuffers() override;
    void pollEvents() override;
    
    void* getWindow() override { return window; }
    void getFramebufferSize(int* width, int* height) override;
    double getTime() override;
    void setVSync(bool enabled) override;
    
private:
    GLFWwindow* window = nullptr;
    std::unique_ptr<Shader> shader;
    RenderBuffers buffers;
    int screenWidth = 800;
    int screenHeight = 600;
    bool vsyncEnabled = true;
    
    void setupGeometry(const GeometryData& geometry);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};