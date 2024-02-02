#include "demo.hpp"

Demo::Demo(const glm::mat4 projection)
    : display_manager(DisplayManager::getInstance()),
      texture_manager(TextureManager::getInstance()),
      renderer(projection),
      camera(glm::vec3(0, 4, 10), 20, 0),
      lighthouse(Transform(glm::vec3(-80.0f, 30.0f, 120.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(1.0f, 1.0f, 1.0f))),
      sailing_ship(Transform(glm::vec3(-20.0f, 0.0f, 30.0f), glm::vec3(90.0f, 0.0f, 0.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f))) {
    generateTerrain();
    entities.push_back(lighthouse.getEntity());
    entities.push_back(sailing_ship.getEntity());
    skybox_day = ShapeFactory::createCubemap(SKYBOX_FACES_DAY);
    skybox_night = ShapeFactory::createCubemap(SKYBOX_FACES_NIGHT);
    water = std::make_shared<Water>(
        glm::vec3(0.0f, -1.0f, 0.0f), 500.0f, 1.0f, 1.0f,
        Material(glm::vec3(0.35f, 0.73f, 0.85f), glm::vec3(0.35f, 0.73f, 0.85f),
                 glm::vec3(0.5f, 0.5f, 0.5f), 101.0f, 1.0f));

    current_ambient = SUNLIGHT_AMBIENT;
    current_diffuse = SUNLIGHT_DIFFUSE;
    current_specular = SUNLIGHT_SPECULAR;
    LightManager::getInstance().addDirlight(LightFactory::createDirectionLight(
        glm::vec3(-1.0f, -0.5f, -1.75f), current_ambient, current_diffuse, current_specular));
}

void Demo::run() {
    float time_passed = 0.0;
    initSound();

    while (!display_manager.windowShouldClose()) {
        float delta_time = static_cast<float>(DisplayManager::getInstance().getDeltaTime());
        time_passed += static_cast<float>(delta_time / 1000.0);

        display_manager.handleEvents();

        sailing_ship.animate(delta_time);
        lighthouse.animate(delta_time);
        updateCamera(delta_time);
        updateLight();
        updateVolume();

        renderer.registerForShadows(sailing_ship.getEntity());
        for (auto& entity : entities) {
            renderer.registerObject(entity);
        }
        for (auto& terrain : terrains) {
            renderer.registerObject(terrain);
        }
        if (current_light > 0.5f) {
            renderer.registerObject(skybox_day);
        } else {
            renderer.registerObject(skybox_night);
        }
        renderer.registerObject(water);

        renderer.render(camera, time_passed);

        display_manager.update();
    }
}

void Demo::generateTerrain() {
    static const float BASE_HEIGHT = 57.0f;
    static const float MAX_HEIGHT = 80.0f;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            TerrainSharedPtr terrain = std::make_shared<Terrain>(
                glm::vec3(-i, BASE_HEIGHT, -j), texture_manager.getTexture("res/grass.png"),
                "res/height_map.jpg", MAX_HEIGHT);
            terrains.push_back(terrain);
        }
    }
}

void Demo::updateVolume() {
    auto dist_from_ship =
        glm::distance(sailing_ship.getEntity()->getPosition(), camera.getPosition());
    if (dist_from_ship == 0.0f)
        sound.setVolume(1.0f);
    else
        sound.setVolume(100 / dist_from_ship);
}

void Demo::initSound() {
    if (!buffer.loadFromFile("res/music/music.wav")) {
        std::cout << "NIE MA MUZI" << std::endl;
    } else {
        sound.setBuffer(buffer);
        sound.setLoop(true);
        sound.play();
    }
}

void Demo::updateLight() {
    const float delta = 0.01f;
    if (display_manager.isKeyPressed(GLFW_KEY_3)) {
        current_light = std::clamp(current_light - delta, 0.0f, 1.0f);
    } else if (display_manager.isKeyPressed(GLFW_KEY_4)) {
        current_light = std::clamp(current_light + delta, 0.0f, 1.0f);
    }
    current_ambient = vec3_lerp(NIGHTLIGHT_AMBIENT, SUNLIGHT_AMBIENT, current_light);
    current_diffuse = vec3_lerp(NIGHTLIGHT_DIFFUSE, SUNLIGHT_DIFFUSE, current_light);
    current_specular = vec3_lerp(NIGHTLIGHT_SPECULAR, SUNLIGHT_SPECULAR, current_light);
    // printf("%f %f %f\n", current_ambient.x, current_ambient.y, current_ambient.z);
    // printf("%f %f %f\n", current_diffuse.x, current_diffuse.y, current_diffuse.z);
    // printf("%f %f %f\n", current_specular.x, current_specular.y, current_specular.z);
    // printf("\n");
    LightManager::getInstance().getDirlights()[0]->setAmbient(current_ambient);
    LightManager::getInstance().getDirlights()[0]->setDiffuse(current_diffuse);
    LightManager::getInstance().getDirlights()[0]->setSpecular(current_specular);
}

void Demo::updateCamera(float delta_time) {
    if (display_manager.isKeyPressed(GLFW_KEY_G)) {
        free_camera = false;
        sailing_ship.setStationary(false);
    } else if (display_manager.isKeyPressed(GLFW_KEY_F)) {
        free_camera = true;
        sailing_ship.setStationary(true);
    }

    if (free_camera)
        camera.move();
    else {
        static const glm::vec3 STEERING_CAMERA_OFFSET_Y = glm::vec3(0, 5, 0);
        static const float STEERING_CAMERA_MIN_DIST = 1.0f;
        static const float STEERING_CAMERA_MAX_DIST = 4.0f;
        static const float SCROLL_SPEED = 0.005f;
        steering_camera_cur_dist =
            std::clamp(steering_camera_cur_dist -
                           (float)display_manager.getScrollY() * SCROLL_SPEED * delta_time,
                       STEERING_CAMERA_MIN_DIST, STEERING_CAMERA_MAX_DIST);
        camera.setPosition(sailing_ship.getEntity()->getPosition() +
                           steering_camera_cur_dist * STEERING_CAMERA_OFFSET_Y +
                           steering_camera_cur_dist * 6.0f * sailing_ship.getFront());
    }
    camera.rotate();
}
