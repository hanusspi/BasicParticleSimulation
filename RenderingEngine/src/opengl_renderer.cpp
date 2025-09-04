#include "opengl_renderer.h"
#include <iostream>

void RenderBuffers::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &normalVBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &instancedModelMatrixVBO);
    glGenBuffers(1, &instancedColorVBO);
}

void RenderBuffers::cleanup() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (normalVBO) glDeleteBuffers(1, &normalVBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (instancedModelMatrixVBO) glDeleteBuffers(1, &instancedModelMatrixVBO);
    if (instancedColorVBO) glDeleteBuffers(1, &instancedColorVBO);
    
    VAO = VBO = normalVBO = EBO = instancedModelMatrixVBO = instancedColorVBO = 0;
}

void RenderBuffers::updateGeometry(const GeometryData& geometry) {
    glBindVertexArray(VAO);
    
    // Update vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 geometry.vertices.size() * sizeof(glm::vec3), 
                 geometry.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Update vertex normals
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 geometry.normals.size() * sizeof(glm::vec3), 
                 geometry.normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);
    
    // Update indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 geometry.indices.size() * sizeof(unsigned int), 
                 geometry.indices.data(), GL_STATIC_DRAW);
}

void RenderBuffers::updateInstanceData(const std::vector<InstanceData>& instances) {
    if (instances.empty()) return;
    
    std::vector<glm::mat4> modelMatrices;
    std::vector<glm::vec3> colors;
    
    modelMatrices.reserve(instances.size());
    colors.reserve(instances.size());
    
    for (const auto& instance : instances) {
        modelMatrices.push_back(instance.modelMatrix);
        colors.push_back(instance.color);
    }
    
    glBindVertexArray(VAO);
    
    // Update model matrix data (4x4 matrix = 4 vec4 attributes)
    glBindBuffer(GL_ARRAY_BUFFER, instancedModelMatrixVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 modelMatrices.size() * sizeof(glm::mat4), 
                 modelMatrices.data(), GL_DYNAMIC_DRAW);
    
    // Setup matrix attributes (locations 2-5)
    for (int i = 0; i < 4; i++) {
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 
                             (void*)(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }
    
    // Update color data
    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 colors.size() * sizeof(glm::vec3), 
                 colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);
}

OpenGLRenderer::OpenGLRenderer() : inputHandler(std::make_unique<InputHandler>()) {}

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
    glEnable(GL_CULL_FACE);     // Enable face culling to remove back faces
    glCullFace(GL_BACK);        // Cull back faces
    glFrontFace(GL_CCW);        // Counter-clockwise vertices are front-facing
    glfwSwapInterval(vsyncEnabled ? 1 : 0);
    
    shader = std::make_unique<Shader>("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    
    // Initialize input handler
    inputHandler->initialize(window);
    
    return true;
}

void OpenGLRenderer::render(const RenderData& data, const Camera& camera) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (data.empty()) return;
    
    shader->use();
    
    int width, height;
    getFramebufferSize(&width, &height);
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
    
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    // Render each geometry type separately
    for (const auto& pair : data.renderGroups) {
        const std::string& geometryName = pair.first;
        const RenderGroup& renderGroup = pair.second;
        
        if (renderGroup.instances.empty() || renderGroup.geometry.empty()) {
            continue;
        }
        
        setupGeometry(geometryName, renderGroup.geometry);
        RenderBuffers& buffers = getOrCreateBuffers(geometryName);
        buffers.updateInstanceData(renderGroup.instances);
        
        glBindVertexArray(buffers.VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 
                              static_cast<GLsizei>(renderGroup.geometry.indices.size()), 
                              GL_UNSIGNED_INT, 0, 
                              static_cast<GLsizei>(renderGroup.instanceCount));
    }
}

void OpenGLRenderer::setupGeometry(const std::string& name, const GeometryData& geometry) {
    RenderBuffers& buffers = getOrCreateBuffers(name);
    buffers.updateGeometry(geometry);
}

RenderBuffers& OpenGLRenderer::getOrCreateBuffers(const std::string& name) {
    auto it = geometryBuffers.find(name);
    if (it == geometryBuffers.end()) {
        geometryBuffers[name].setup();
        return geometryBuffers[name];
    }
    return it->second;
}

void OpenGLRenderer::shutdown() {
    for (auto& pair : geometryBuffers) {
        pair.second.cleanup();
    }
    geometryBuffers.clear();
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

// Input handling methods
void OpenGLRenderer::setCamera(std::shared_ptr<Camera> camera) {
    if (inputHandler) {
        inputHandler->setCamera(camera);
    }
}

void OpenGLRenderer::processInput(float deltaTime) {
    if (inputHandler && window) {
        inputHandler->processKeyboard(window, deltaTime);
    }
}

bool OpenGLRenderer::isCloseRequested() const {
    return inputHandler ? inputHandler->shouldClose() : false;
}

bool OpenGLRenderer::isVSyncToggled() const {
    return inputHandler ? inputHandler->isVSyncToggled() : false;
}

void OpenGLRenderer::resetVSyncToggle() {
    if (inputHandler) {
        inputHandler->resetVSyncToggle();
    }
}