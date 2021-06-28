#pragma once

#include <iostream>
#include <vector>

#include "opengl_all.hpp"

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float alpha;
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float alpha)
        : ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          shininess(shininess),
          alpha(alpha) {}
};

using MaterialSPtr = std::shared_ptr<Material>;