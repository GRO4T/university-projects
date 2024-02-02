#pragma once

#include <algorithm>

#include "opengl_all.hpp"

using Key = unsigned int;
using GLFWKeyState = unsigned int;

class DisplayManager {
public:
    ~DisplayManager() { close(); }
    static DisplayManager& getInstance();
    static void init();
    void createDisplay(int width, int height);
    void handleEvents();
    void update();
    bool windowShouldClose();
    GLFWwindow* getWindow() { return window; }

    bool isKeyPressed(Key key);

    double getMouseDX() const { return mouse_dx; }
    double getMouseDY() const { return mouse_dy; }
    double getScrollX() const { return scroll_x; }
    double getScrollY() const { return scroll_y; }
    void updateMouseDelta(double pos_x, double pos_y);
    void updateScroll(double offset_x, double offset_y);
    void increaseMouseSensitivity(double delta) {
        mouse_sensitivity_x = std::clamp(mouse_sensitivity_x + delta, 0.0, MAX_MOUSE_SENSITIVITY);
        mouse_sensitivity_y = std::clamp(mouse_sensitivity_y + delta, 0.0, MAX_MOUSE_SENSITIVITY);
    }
    void decreaseMouseSensitivity(double delta) {
        mouse_sensitivity_x = std::clamp(mouse_sensitivity_x - delta, 0.0, MAX_MOUSE_SENSITIVITY);
        mouse_sensitivity_y = std::clamp(mouse_sensitivity_y - delta, 0.0, MAX_MOUSE_SENSITIVITY);
    }

    double getDeltaTime() const { return delta_time; }

private:
    enum class KeyState;

    const double MAX_MOUSE_SENSITIVITY = 4.0f;

    DisplayManager() {}
    DisplayManager(const DisplayManager&) = delete;
    DisplayManager& operator=(const DisplayManager&) = delete;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void cursorPositionCallback(GLFWwindow* window, double pos_x, double pos_y);
    static void scrollCallback(GLFWwindow* window, double offset_x, double offset_y);

    void close();

    GLFWwindow* window;
    int window_w, window_h;
    double last_update;
    double delta_time;

    bool first_mouse = true;
    double mouse_sensitivity_y = 1.0f;
    double mouse_sensitivity_x = 1.0f;
    double last_mouse_x, last_mouse_y;
    double mouse_dx, mouse_dy;
    double scroll_x, scroll_y;
};