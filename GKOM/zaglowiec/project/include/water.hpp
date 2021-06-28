#pragma once

#include <vector>

#include "model/material.hpp"
#include "model/model.hpp"
#include "utils.hpp"

class Water {
public:
    Water(const glm::vec3& root, float size, float segment_size, float wave_size,
          Material material);

    const Model& getModel() const { return model; }
    const float getWaveSize() const { return wave_size; }
    const Material& getMaterial() const { return material; }
    utils::Transform getTransform() const;

private:
    glm::vec3 root;
    float wave_size;
    Model model;
    Material material;
};

using WaterPtr = std::unique_ptr<Water>;
using WaterSharedPtr = std::shared_ptr<Water>;