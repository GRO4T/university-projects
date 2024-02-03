#pragma once

#include "lighting/lighting.hpp"
#include "utils.hpp"

class LightFactory {
public:
    static LightSharedPtr createDirectionLight(glm::vec3 direction, glm::vec3 ambient,
                                               glm::vec3 diffuse, glm::vec3 specular);
    static LightSharedPtr createPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                                           glm::vec3 specular, float constant = 1.0f,
                                           float linear = 0.045, float quadratic = 0.0075);
    static LightSharedPtr createSpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                                          glm::vec3 specular, glm::vec3 direction, float cutOff,
                                          float outerCutOff, float constant = 1.0f,
                                          float linear = 0.09f, float quadratic = 0.032f);
};