#include "lighting/light_factory.hpp"

LightSharedPtr LightFactory::createDirectionLight(glm::vec3 direction, glm::vec3 ambient,
                                                  glm::vec3 diffuse, glm::vec3 specular) {
    return std::make_shared<DirLight>(direction, ambient, diffuse, specular);
}

LightSharedPtr LightFactory::createSpotLight(glm::vec3 position, glm::vec3 ambient,
                                             glm::vec3 diffuse, glm::vec3 specular,
                                             glm::vec3 direction, float cutOff, float outerCutOff,
                                             float constant, float linear, float quadratic) {
    return std::make_shared<SpotLight>(position, ambient, diffuse, specular, direction, cutOff,
                                       outerCutOff, constant, linear, quadratic);
}

LightSharedPtr LightFactory::createPointLight(glm::vec3 position, glm::vec3 ambient,
                                              glm::vec3 diffuse, glm::vec3 specular, float constant,
                                              float linear, float quadratic) {
    return std::make_shared<PointLight>(position, ambient, diffuse, specular, constant, linear,
                                        quadratic);
}