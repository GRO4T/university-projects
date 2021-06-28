#pragma once
#include "opengl_all.hpp"

class Camera {
public:
    Camera() : position(0, 0, -10) {}
    Camera(const glm::vec3& position, float pitch, float yaw)
        : position(position), pitch(pitch), yaw(yaw) {}
    void move();
    void rotate();
    const glm::vec3& getPosition() const { return position; }
    void setPosition(const glm::vec3& val) { position = val; }
    double getPitch() const { return pitch; }
    double getYaw() const { return yaw; }
    double getRoll() const { return roll; }
    glm::vec3 getFront() const {
        glm::vec3 front;
        front.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front.y = static_cast<float>(sin(glm::radians(pitch)));
        front.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        return glm::normalize(front);
    }

private:
    glm::vec3 position;
    double pitch = 0;
    double yaw = -180;
    double roll = 0;
    double speed = 0.02f;
};