#pragma once

#include <vector>

#include "../lighting/lighting.hpp"
#include "shader/water_shader.hpp"
#include "water.hpp"

class WaterRenderer {
public:
    WaterRenderer(const glm::mat4& projection_matrix);
    void render(WaterSharedPtr& water, const Camera& camera, float time);

private:
    WaterShader shader;
};