#include "camera.hpp"

#include <iostream>

#include "display_manager.hpp"

void Camera::move() {
    DisplayManager& dm = DisplayManager::getInstance();
    double real_speed = dm.getDeltaTime() * speed;
    if (dm.isKeyPressed(GLFW_KEY_W)) {
        position.x += (float)(cos(glm::radians(pitch)) * sin(glm::radians(yaw)) * real_speed);
        position.y -= (float)(sin(glm::radians(pitch)) * real_speed);
        position.z -= (float)(cos(glm::radians(pitch)) * cos(glm::radians(yaw)) * real_speed);
    }
    if (dm.isKeyPressed(GLFW_KEY_S)) {
        position.x -= (float)(cos(glm::radians(pitch)) * sin(glm::radians(yaw)) * real_speed);
        position.y += (float)(sin(glm::radians(pitch)) * real_speed);
        position.z += (float)(cos(glm::radians(pitch)) * cos(glm::radians(yaw)) * real_speed);
    }
    if (dm.isKeyPressed(GLFW_KEY_D)) {
        position.x += (float)(cos(glm::radians(yaw)) * real_speed);
        position.z += (float)(sin(glm::radians(yaw)) * real_speed);
    }
    if (dm.isKeyPressed(GLFW_KEY_A)) {
        position.x -= (float)(cos(glm::radians(yaw)) * real_speed);
        position.z -= (float)(sin(glm::radians(yaw)) * real_speed);
    }
}

void Camera::rotate() {
    DisplayManager& dm = DisplayManager::getInstance();
    double real_speed = dm.getDeltaTime() * speed;
    yaw += dm.getMouseDX() * real_speed;
    pitch -= dm.getMouseDY() * real_speed;
}