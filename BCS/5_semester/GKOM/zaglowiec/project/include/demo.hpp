#pragma once

#include <SFML/Audio.hpp>
#include <cmath>
#include <entity/complex/lighthouse.hpp>
#include <entity/complex/sailing_ship.hpp>
#include <renderer/master_renderer.hpp>
#include <texture/texture_manager.hpp>
#include <vector>

#include "camera.hpp"
#include "display_manager.hpp"
#include "entity/shape_factory.hpp"
#include "lighting/light_factory.hpp"
#include "lighting/light_manager.hpp"
#include "model/material.hpp"
#include "model/shape.hpp"
#include "model/textured_model.hpp"
#include "renderer/entity_renderer.hpp"
#include "stb_image.h"
#include "utils.hpp"

using namespace utils;

class Demo {
public:
    Demo(const glm::mat4 projection);
    void run();

private:
    const std::vector<std::string> SKYBOX_FACES_DAY{
        "res/skybox/day/right.jpg",  "res/skybox/day/left.jpg",  "res/skybox/day/top.jpg",
        "res/skybox/day/bottom.jpg", "res/skybox/day/front.jpg", "res/skybox/day/back.jpg"};
    const std::vector<std::string> SKYBOX_FACES_NIGHT{
        "res/skybox/night/right.jpg",  "res/skybox/night/left.jpg",  "res/skybox/night/top.jpg",
        "res/skybox/night/bottom.jpg", "res/skybox/night/front.jpg", "res/skybox/night/back.jpg"};
    const glm::vec3 SUNLIGHT_AMBIENT = glm::vec3(0.33f, 0.33f, 0.33f);
    const glm::vec3 SUNLIGHT_DIFFUSE = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 SUNLIGHT_SPECULAR = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 NIGHTLIGHT_AMBIENT = glm::vec3(0.05f, 0.05f, 0.05f);
    const glm::vec3 NIGHTLIGHT_DIFFUSE = glm::vec3(0.2f, 0.2f, 0.2f);
    const glm::vec3 NIGHTLIGHT_SPECULAR = glm::vec3(0.5f, 0.5f, 0.5f);

    void generateTerrain();

    void updateVolume();
    void initSound();

    void updateLight();
    void updateCamera(float delta_time);

    DisplayManager& display_manager = DisplayManager::getInstance();
    TextureManager& texture_manager = TextureManager::getInstance();

    MasterRenderer renderer;
    Camera camera;
    std::vector<EntitySPtr> entities;
    std::vector<TerrainSharedPtr> terrains;

    Lighthouse lighthouse;
    SailingShip sailing_ship;
    CubemapSharedPtr skybox_day;
    CubemapSharedPtr skybox_night;
    WaterSharedPtr water;

    float current_light = 1.0;
    glm::vec3 current_ambient;
    glm::vec3 current_diffuse;
    glm::vec3 current_specular;
    sf::SoundBuffer buffer;

    bool free_camera = true;
    float steering_camera_cur_dist = 1.0f;

    sf::Sound sound;
};