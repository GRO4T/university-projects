#pragma once

#include <vector>

#include "model/material.hpp"
#include "model/model.hpp"
#include "texture/texture.hpp"

class Terrain {
public:
    Terrain(const glm::vec3& pos, TextureSPtr texture,
            const std::string& height_map = "res/default_height_map.jpg", float max_height = 40,
            MaterialSPtr material = DEFAULT_MATERIAL);

    const Texture& getTexture() const { return *texture; }
    const Model& getModel() const { return model; }
    const Material& getMaterial() const { return *material; }
    float getX() const { return x; }
    float getZ() const { return z; }

private:
    void generate(const std::string& height_map_path);
    float getHeight(int x, int z, const utils::Image& height_map);
    glm::vec3 calculateNormal(int x, int z, const utils::Image& height_map);

    static constexpr float SIZE = 800;
    float max_height;
    static constexpr float MAX_PIXEL_COLOR = 256 * 256 * 256;
    static const MaterialSPtr DEFAULT_MATERIAL;

    float x, z;
    float base_height;
    IndexedModel model;
    TextureSPtr texture;
    MaterialSPtr material;
};

using TerrainPtr = std::unique_ptr<Terrain>;
using TerrainSharedPtr = std::shared_ptr<Terrain>;