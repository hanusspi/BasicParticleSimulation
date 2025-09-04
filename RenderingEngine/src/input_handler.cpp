#include "input_handler.h"
#include <GLFW/glfw3.h>
#include <iostream>

InputHandler* InputHandler::instance = nullptr;

InputHandler::InputHandler() {
    instance = this;
}

void InputHandler::initialize(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    lastX = width / 2.0f;
    lastY = height / 2.0f;
}

void InputHandler::setCamera(std::shared_ptr<Camera> cam) {
    camera = cam;
}

void InputHandler::processKeyboard(GLFWwindow* window, float deltaTime) {
    if (keys[GLFW_KEY_ESCAPE]) {
        closeRequested = true;
    }
    
    if (keys[GLFW_KEY_V] && !vKeyPressed) {
        vsyncToggled = true;
        vKeyPressed = true;
    }
    if (!keys[GLFW_KEY_V]) {
        vKeyPressed = false;
    }
    
    if (camera) {
        if (keys[GLFW_KEY_W])
            camera->processKeyboard(Camera::FORWARD, deltaTime);
        if (keys[GLFW_KEY_S])
            camera->processKeyboard(Camera::BACKWARD, deltaTime);
        if (keys[GLFW_KEY_A])
            camera->processKeyboard(Camera::LEFT, deltaTime);
        if (keys[GLFW_KEY_D])
            camera->processKeyboard(Camera::RIGHT, deltaTime);
        if (keys[GLFW_KEY_Q])
            camera->processKeyboard(Camera::UP, deltaTime);
        if (keys[GLFW_KEY_E])
            camera->processKeyboard(Camera::DOWN, deltaTime);
        if (keys[GLFW_KEY_R]) {
            camera->setPosition(glm::vec3(0.0f, 0.0f, 2.0f));
        }
    }
}

void InputHandler::processMouse(double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }
    
    float xOffset = static_cast<float>(xpos) - lastX;
    float yOffset = lastY - static_cast<float>(ypos);
    
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    
    if (camera) {
        camera->processMouseMovement(xOffset, yOffset);
    }
}

void InputHandler::processScroll(double yoffset) {
    if (camera) {
        camera->processMouseScroll(static_cast<float>(yoffset));
    }
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (instance && key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            instance->keys[key] = true;
        else if (action == GLFW_RELEASE)
            instance->keys[key] = false;
    }
}

void InputHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (instance) {
        instance->processMouse(xpos, ypos);
    }
}

void InputHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (instance) {
        instance->processScroll(yoffset);
    }
}