/*
 * RenderingEngine Example
 * 
 * This example demonstrates the basic usage of the RenderingEngine library.
 * It creates a simple scene with spheres and cubes, and shows how to:
 * 1. Initialize the renderer
 * 2. Create and manage geometry
 * 3. Add objects to the world
 * 4. Handle camera movement
 * 5. Render the scene
 */

#include "opengl_renderer.h"
#include "world.h"
#include "camera.h"
#include <iostream>
#include <memory>

int main() {
    // Initialize renderer
    auto renderer = std::make_unique<OpenGLRenderer>();
    if (!renderer->initialize(1200, 900, "RenderingEngine Example")) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    // Create world and camera
    World world;
    world.initialize();
    
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));  // Start 5 units back

    // Create a colorful scene
    std::cout << "Creating scene..." << std::endl;
    
    // Add some spheres
    world.addSphere(glm::vec3(-2.0f, 0.0f, 0.0f), 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)); // Red sphere
    world.addSphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, glm::vec3(0.0f, 1.0f, 0.0f));   // Green sphere
    world.addSphere(glm::vec3(2.0f, 0.0f, 0.0f), 0.7f, glm::vec3(0.0f, 0.0f, 1.0f));   // Blue sphere
    
    // Add some cubes with different transforms
    world.addCube(glm::vec3(-1.0f, 2.0f, 0.0f),                    // Position
                  glm::vec3(0.5f, 0.5f, 0.5f),                     // Scale
                  glm::vec3(0.0f, 0.0f, 45.0f * 3.14159f / 180.0f), // Rotation (45 degrees)
                  glm::vec3(1.0f, 1.0f, 0.0f));                    // Yellow
    
    world.addCube(glm::vec3(1.0f, 2.0f, 0.0f),                     // Position
                  glm::vec3(0.3f, 1.0f, 0.3f),                     // Scale
                  glm::vec3(45.0f * 3.14159f / 180.0f, 0.0f, 0.0f), // Rotation
                  glm::vec3(1.0f, 0.0f, 1.0f));                    // Magenta
    
    // Add a container to frame the scene
    world.addContainer(glm::vec3(3.0f, 2.5f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    std::cout << "Scene created! Use WASD to move, QE for up/down, mouse to look around." << std::endl;
    std::cout << "Press ESC to exit." << std::endl;

    // Main render loop
    RenderData renderData;
    double lastTime = renderer->getTime();
    
    while (!renderer->shouldClose()) {
        double currentTime = renderer->getTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Handle input (basic WASD movement)
        GLFWwindow* window = static_cast<GLFWwindow*>(renderer->getWindow());
        
        // Simple camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(Camera::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(Camera::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(Camera::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(Camera::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.processKeyboard(Camera::DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.processKeyboard(Camera::UP, deltaTime);
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Update camera aspect ratio
        int width, height;
        renderer->getFramebufferSize(&width, &height);
        camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));

        // Get render data and render
        world.getRenderData(renderData);
        renderer->render(renderData, camera);
        
        renderer->swapBuffers();
        renderer->pollEvents();
    }

    // Cleanup
    renderer->shutdown();
    
    std::cout << "Example completed successfully!" << std::endl;
    return 0;
}

/*
 * To build this example:
 * 
 * 1. Make sure you have GLAD, GLFW, and GLM set up
 * 2. Build RenderingEngine as a library
 * 3. Compile this example linking against RenderingEngine
 * 
 * CMakeLists.txt example:
 * 
 * add_subdirectory(path/to/RenderingEngine)
 * add_executable(example example.cpp)
 * target_link_libraries(example RenderingEngine)
 */