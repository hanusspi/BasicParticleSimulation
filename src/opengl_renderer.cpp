#include "opengl_renderer.h"
#include <iostream>

void RenderBuffers::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &instancedVBO);
    glGenBuffers(1, &instancedColorVBO);
}

void RenderBuffers::cleanup() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (instancedVBO) glDeleteBuffers(1, &instancedVBO);
    if (instancedColorVBO) glDeleteBuffers(1, &instancedColorVBO);
    
    VAO = VBO = EBO = instancedVBO = instancedColorVBO = 0;
}

void RenderBuffers::updateInstanceData(const std::vector<InstanceData>& instances) {
    if (instances.empty()) return;
    
    std::vector<glm::vec4> positions;
    std::vector<glm::vec3> colors;
    positions.reserve(instances.size());
    colors.reserve(instances.size());
    
    for (const auto& instance : instances) {
        positions.push_back(instance.positionScale);
        colors.push_back(instance.color);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 positions.size() * sizeof(glm::vec4), 
                 positions.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 colors.size() * sizeof(glm::vec3), 
                 colors.data(), GL_DYNAMIC_DRAW);
}

OpenGLRenderer::OpenGLRenderer() = default;

OpenGLRenderer::~OpenGLRenderer() {
    shutdown();
}

bool OpenGLRenderer::initialize(int width, int height, const char* title) {
    screenWidth = width;
    screenHeight = height;
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(vsyncEnabled ? 1 : 0);
    
    shader = std::make_unique<Shader>("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    buffers.setup();
    
    return true;
}

void OpenGLRenderer::render(const RenderData& data, const Camera& camera) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader->use();
    
    int width, height;
    getFramebufferSize(&width, &height);
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
    
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    if (!data.instances.empty() && !data.geometry.indices.empty()) {
        setupGeometry(data.geometry);
        buffers.updateInstanceData(data.instances);
        
        glBindVertexArray(buffers.VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 
                              static_cast<GLsizei>(data.geometry.indices.size()), 
                              GL_UNSIGNED_INT, 0, 
                              static_cast<GLsizei>(data.instanceCount));
    }
}

void OpenGLRenderer::setupGeometry(const GeometryData& geometry) {
    glBindVertexArray(buffers.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 geometry.vertices.size() * sizeof(glm::vec3), 
                 geometry.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 geometry.indices.size() * sizeof(unsigned int), 
                 geometry.indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers.instancedVBO);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers.instancedColorVBO);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
}

void OpenGLRenderer::shutdown() {
    buffers.cleanup();
    shader.reset();
    
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

bool OpenGLRenderer::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void OpenGLRenderer::swapBuffers() {
    if (window) {
        glfwSwapBuffers(window);
    }
}

void OpenGLRenderer::pollEvents() {
    glfwPollEvents();
}

void OpenGLRenderer::getFramebufferSize(int* width, int* height) {
    if (window) {
        glfwGetFramebufferSize(window, width, height);
    } else {
        *width = screenWidth;
        *height = screenHeight;
    }
}

double OpenGLRenderer::getTime() {
    return glfwGetTime();
}

void OpenGLRenderer::setVSync(bool enabled) {
    vsyncEnabled = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

void OpenGLRenderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}