#include "demo.hpp"

using namespace utils;

const GLuint WIDTH = 1440, HEIGHT = 900;
const float FOV = 45.0f;
const float Z_NEAR = 0.1f;
const float Z_FAR = 400.0f;

int main() {
    try {
        DisplayManager::init();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }

    DisplayManager& display_manager = DisplayManager::getInstance();

    try {
        DisplayManager::getInstance().createDisplay(WIDTH, HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);

        glm::mat4 projection = glm::mat4(1.0f);
        projection =
            glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, Z_NEAR, Z_FAR);

        Demo demo(projection);
        demo.run();
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
