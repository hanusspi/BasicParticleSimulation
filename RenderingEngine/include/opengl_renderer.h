#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "renderer.h"
#include "shader.h"
#include "input_handler.h"

struct RenderBuffers {
    unsigned int VAO = 0;
    unsigned int VBO = 0;        // Vertex positions
    unsigned int normalVBO = 0;  // Vertex normals
    unsigned int EBO = 0;        // Element indices
    unsigned int instancedModelMatrixVBO = 0; // 4x4 model matrices
    unsigned int instancedColorVBO = 0;       // Instance colors
    
    void setup();
    void cleanup();
    void updateGeometry(const GeometryData& geometry);
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
    
    // Input handling
    void setCamera(std::shared_ptr<Camera> camera) override;
    void processInput(float deltaTime) override;
    bool isCloseRequested() const override;
    bool isVSyncToggled() const override;
    void resetVSyncToggle() override;
    
private:
    GLFWwindow* window = nullptr;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<InputHandler> inputHandler;
    std::unordered_map<std::string, RenderBuffers> geometryBuffers;
    int screenWidth = 800;
    int screenHeight = 600;
    bool vsyncEnabled = true;
    
    void setupGeometry(const std::string& name, const GeometryData& geometry);
    RenderBuffers& getOrCreateBuffers(const std::string& name);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};