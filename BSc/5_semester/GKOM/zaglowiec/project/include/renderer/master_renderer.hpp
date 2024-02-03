#pragma once

#include "cubemap.hpp"
#include "entity/entity.hpp"
#include "entity_renderer.hpp"
#include "lighting/lighting.hpp"
#include "shader/entity_shader.hpp"
#include "shader/skybox_shader.hpp"
#include "shader/terrain_shader.hpp"
#include "skybox_renderer.hpp"
#include "terrain.hpp"
#include "terrain_renderer.hpp"
#include "water_renderer.hpp"

class MasterRenderer {
public:
    MasterRenderer(const glm::mat4& projection_matrix)
        : entity_renderer(projection_matrix),
          terrain_renderer(projection_matrix),
          skybox_renderer(projection_matrix),
          water_renderer(projection_matrix) {}

    void registerObject(TerrainSharedPtr terrain) { terrains.push_back(terrain); }
    void registerObject(EntitySPtr entity) { entities.push_back(entity); }
    void registerForShadows(EntitySPtr entity) { entity_renderer.registerForShadows(entity); }
    void registerObject(CubemapSharedPtr cubemap) { this->skybox = cubemap; }
    void registerObject(WaterSharedPtr water_) { this->water = water_; }

    void render(const Camera& camera, float time) {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        entity_renderer.render(entities, camera);
        terrain_renderer.render(terrains, camera);
        skybox_renderer.render(skybox, camera);
        water_renderer.render(water, camera, time);

        entities.clear();
        terrains.clear();
    }

    EntityRenderer getEntityRenderer() { return entity_renderer; }

private:
    std::vector<TerrainSharedPtr> terrains;
    std::vector<EntitySPtr> entities;
    CubemapSharedPtr skybox;
    WaterSharedPtr water;

    EntityRenderer entity_renderer;
    TerrainRenderer terrain_renderer;
    SkyboxRenderer skybox_renderer;
    WaterRenderer water_renderer;
};