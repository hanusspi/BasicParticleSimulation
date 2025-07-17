#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "shader.h"
#include "geometry.h"
#include "sim.h"

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool vsyncEnabled = true;
double lastTime = 0.0;
double frameTime = 0.0;
int frameCount = 0;

double inOut = 0.0;
double upDown = 0.0;
double rightLeft = 0.0;


// Callback function for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Handle input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    static bool vKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && !vKeyPressed) {
        vsyncEnabled = !vsyncEnabled;
        glfwSwapInterval(vsyncEnabled ? 1 : 0); // 1 = on, 0 = off
        std::cout << "VSync: " << (vsyncEnabled ? "ON" : "OFF") << std::endl;
        vKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
        vKeyPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        upDown += 0.1; // Rotate up
    }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		upDown -= 0.1; // Rotate down
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rightLeft -= 0.1; // Rotate left
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rightLeft += 0.1; // Rotate right
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		inOut += 0.1; // Move in
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		inOut -= 0.1; // Move out
	}
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		inOut = 0.0; // Reset inOut
		upDown = 0.0; // Reset upDown
		rightLeft = 0.0; // Reset rightLeft
    }
}

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Particle Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    sphere mySphere(20);

    cube myCube;

    std::vector<glm::vec3> vertices = mySphere.getVertices();
    std::vector<std::array<int, 3>> indices = mySphere.getIndices();

    //std::vector<glm::vec3> vertices = myCube.getVertices();
    //std::vector<std::array<int, 3>> indices = myCube.getIndices();

    // Generate instanced data for 100 spheres
    const int NUM_SPHERES = 1000;
	std::vector<glm::vec4> instancedData(NUM_SPHERES);
	std::vector<glm::vec3> instancedColors(NUM_SPHERES);

	for (int i = 0; i < NUM_SPHERES; ++i) {
        float x = (i % 10) * 3.0f - 15.0f;  // 10x10 grid
        float y = (i / 10) * 3.0f - 15.0f;
        float z = (i % 10) * 3.0f - 15.0f;
        float scale = 0.5f + ((float)rand() / RAND_MAX) * 1.0f;

		instancedData[i] = glm::vec4(x, y, z, scale);
		instancedColors[i] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		//instancedColors[i] = glm::vec3(1.0f, 1.0f, 1.0f); // White color for all spheres
    }

    unsigned int VBO, VAO, EBO;
	unsigned int instancedVBO, instancedColorVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &instancedVBO);
	glGenBuffers(1, &instancedColorVBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);


    std::vector<unsigned int> flatIndices;
    for (const auto& index : indices) {
        flatIndices.push_back(index[0]);
        flatIndices.push_back(index[1]);
        flatIndices.push_back(index[2]);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, flatIndices.size() * sizeof(unsigned int), flatIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, instancedData.size() * sizeof(glm::vec4), instancedData.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1); // Enable instancing for this attribute

	glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
	glBufferData(GL_ARRAY_BUFFER, instancedColors.size() * sizeof(glm::vec3), instancedColors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    Sim sim;
	sim.init(&instancedData, &instancedColors);


    // Render loop
    while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		frameTime = currentTime - lastTime;
		lastTime = currentTime;
		frameCount++;

        // Input
        processInput(window);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		float apsectRatio = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 30), glm::vec3(rightLeft, upDown, inOut), glm::vec3(0, 1, 0));
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), apsectRatio, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        for (int i = 0; i < NUM_SPHERES; i++) {
            // Update positions (e.g., rotate in circle)
            float time = glfwGetTime();
            float angle = (i / (float)NUM_SPHERES) * 2.0f * 3.14159f + time;
            instancedData[i].x = cos(angle) * 10.0f;
            instancedData[i].y = sin(angle) * 10.0f;
            instancedData[i].z = 0.0f;
            // Keep same scale: instanceData[i].w unchanged
        }

        glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
        glBufferData(GL_ARRAY_BUFFER, instancedData.size() * sizeof(glm::vec4), instancedData.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, flatIndices.size(), GL_UNSIGNED_INT, 0, NUM_SPHERES);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}