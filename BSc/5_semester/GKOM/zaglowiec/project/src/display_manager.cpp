#include "display_manager.hpp"

#include <iostream>

#include "opengl_all.hpp"

DisplayManager& DisplayManager::getInstance() {
    static DisplayManager instance;
    return instance;
}

void DisplayManager::init() {
    if (glfwInit() != GL_TRUE) {
        std::cout << "GLFW" << std::endl;
        throw std::runtime_error("GLFW initialization failed");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
}
void DisplayManager::createDisplay(int width, int height) {
    window_w = width;
    window_h = height;
    window = glfwCreateWindow(width, height, "GKOM - OpenGL 05", nullptr, nullptr);
    if (window == nullptr) throw std::runtime_error("GLFW window not created");
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW Initialization failed");
    }
}
void DisplayManager::handleEvents() {
    if (isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    glfwPollEvents();
}

void DisplayManager::update() {
    float now = (float)glfwGetTime() * 1000.0f;
    delta_time = now - last_update;
    last_update = now;
    // std::cout << delta_time << "s" << std::endl;
    // std::cout << "FPS: " << 1000.0f / delta_time << std::endl;
    // std::cout << "mouse sensitivity: " << mouse_sensitivity_x << std::endl;
    glfwSwapBuffers(window);
    mouse_dx = mouse_dy = 0;
    scroll_x = scroll_y = 0;
}

bool DisplayManager::windowShouldClose() { return glfwWindowShouldClose(window); }

void DisplayManager::close() {
    if (!windowShouldClose()) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    glfwTerminate();
}

void DisplayManager::cursorPositionCallback(GLFWwindow* window, double pos_x, double pos_y) {
    DisplayManager::getInstance().updateMouseDelta(pos_x, pos_y);
}

void DisplayManager::updateMouseDelta(double pos_x, double pos_y) {
    if (first_mouse) {
        last_mouse_x = pos_x;
        last_mouse_y = pos_y;
        first_mouse = false;
    }
    mouse_dx = (pos_x - last_mouse_x) * mouse_sensitivity_x;
    mouse_dy = (last_mouse_y - pos_y) * mouse_sensitivity_y;
    last_mouse_x = pos_x;
    last_mouse_y = pos_y;
}

bool DisplayManager::isKeyPressed(Key key) { return glfwGetKey(window, key) == GLFW_PRESS; }

void DisplayManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_1) {
        getInstance().decreaseMouseSensitivity(0.01);
    } else if (key == GLFW_KEY_2) {
        getInstance().increaseMouseSensitivity(0.01);
    }
}
void DisplayManager::scrollCallback(GLFWwindow* window, double offset_x, double offset_y) {
    DisplayManager::getInstance().updateScroll(offset_x, offset_y);
}
void DisplayManager::updateScroll(double offset_x, double offset_y) {
    scroll_x = offset_x;
    scroll_y = offset_y;
}
