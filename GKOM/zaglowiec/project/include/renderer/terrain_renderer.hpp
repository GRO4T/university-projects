#pragma once

#include <vector>

#include "lighting/lighting.hpp"
#include "shader/terrain_shader.hpp"
#include "terrain.hpp"

class TerrainRenderer {
public:
    TerrainRenderer(const glm::mat4& projection_matrix);
    void render(std::vector<TerrainSharedPtr>& terrains, const Camera& camera);

private:
    void unbind(const Model& model);
    void prepareInstance(const Terrain& terrain);

    TerrainShader shader;
};