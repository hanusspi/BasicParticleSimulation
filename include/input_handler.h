#pragma once
#include <memory>

struct GLFWwindow;

#include "camera.h"

class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;
    
    void initialize(GLFWwindow* window);
    void setCamera(std::shared_ptr<Camera> cam);
    
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouse(double xpos, double ypos);
    void processScroll(double yoffset);
    
    bool shouldClose() const { return closeRequested; }
    bool isVSyncToggled() const { return vsyncToggled; }
    void resetVSyncToggle() { vsyncToggled = false; }
    
    static InputHandler* getInstance() { return instance; }
    
private:
    std::shared_ptr<Camera> camera;
    bool keys[1024] = {false};
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;
    bool closeRequested = false;
    bool vsyncToggled = false;
    bool vKeyPressed = false;
    
    static InputHandler* instance;
    
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};