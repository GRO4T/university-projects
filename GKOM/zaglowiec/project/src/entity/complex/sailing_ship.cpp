#include "entity/complex/sailing_ship.hpp"

#include "display_manager.hpp"
#include "lighting/light_factory.hpp"
#include "lighting/light_manager.hpp"
#include "utils.hpp"

SailingShip::SailingShip(const Transform& t) {
    front = utils::rotate_y(front, -t.rotation.z);
    wood_texture = TextureManager::getInstance().getTexture(WOOD_TEXTURE_PATH);
    // ship deck
    material = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                          glm::vec3(0.0f, 0.0f, 0.0f), 32.0f, 1.0f);
    root =
        ShapeFactory::createCylinder(t, WOOD_TEXTURE_PATH, material, DECK_DIAMETER, DECK_LENGTH, 3);
    RailingsGenerator railings_generator;
    railings_generator.generate(root, wood_texture);
    generateMast();
    generateSteeringWheel();
    generateTorches();
}

void SailingShip::RailingsGenerator::generate(EntitySPtr root, TextureSPtr texture) {
    generateVerticalPoles(root, texture);
    generateHorizontalPoles(root, texture);
}

SailingShip::RailingsGenerator::RailingsGenerator() {
    x_offset = DECK_DIAMETER * 3 / (4 * (float)sqrt(3)) - rd.PADDING;
    y_offset = DECK_LENGTH / 2.0f - rd.PADDING;
    z_offset = -(DECK_DIAMETER * 0.8f - rd.POLE_HEIGHT) / 2.0f;
}

void SailingShip::RailingsGenerator::generateVerticalPoles(EntitySPtr root, TextureSPtr texture) {
    ModelPtr pole_model = std::make_unique<Model>();
    pole_model->load(shape::cuboid(rd.POLE_WIDTH, rd.POLE_HEIGHT, rd.POLE_WIDTH));
    TexturedModelSPtr pole_textured_model =
        std::make_shared<TexturedModel>(std::move(pole_model), texture);
    float step_x = 2 * x_offset / (rd.NUM_POLES_X - 1);
    float step_y = 2 * y_offset / (rd.NUM_POLES_Y - 1);
    Transform t(glm::vec3(0, 0, z_offset), glm::vec3(90.0f, 0, 0), glm::vec3(1, 1, 1));
    for (float i = 0; i < x_offset + EPSILON; i += step_x) {
        for (float j = 0; j < y_offset + EPSILON; j += step_y) {
            // this ensures that only poles on the edge are created
            if (j >= 0 && j + step_y < y_offset + EPSILON && i + step_x < x_offset + EPSILON)
                continue;
            // save these values so we will be able to perfectly align the railings
            if (i + step_x > x_offset + EPSILON) max_pole_x = i;
            if (j + step_y > y_offset + EPSILON) max_pole_y = j;
            // we mirror the pole's positions so everything is symmetrical
            for (auto& coord : MIRROR_COORDS) {
                t.position = glm::vec3(coord.first * i, coord.second * j, t.position.z);
                root->addChild(std::make_shared<Entity>(pole_textured_model, t));
            }
        }
    }
}

void SailingShip::RailingsGenerator::generateHorizontalPoles(EntitySPtr root, TextureSPtr texture) {
    // handrails (first create handrails front and back then those on the sides of the ship)
    for (int i = 0; i < 2; ++i) {
        ModelPtr pole_model = std::make_unique<Model>();
        pole_model->load(
            shape::cuboid(rd.POLE_WIDTH, i == 0 ? 2 * x_offset : 2 * y_offset, rd.POLE_WIDTH));
        TexturedModelSPtr pole_textured_model =
            std::make_shared<TexturedModel>(std::move(pole_model), texture);
        Transform t(glm::vec3(0, 0, -rd.POLE_HEIGHT / 2.0f - rd.POLE_WIDTH / 2.0f + z_offset),
                    glm::vec3(), glm::vec3(1, 1, 1));
        if (i == 0) {  // front and back
            t.rotation.z = 90.0f;
            t.position = glm::vec3(0, max_pole_y, t.position.z);
            root->addChild(std::make_shared<Entity>(pole_textured_model, t));
            t.position.y = -max_pole_y;
            root->addChild(std::make_shared<Entity>(pole_textured_model, t));
        } else {  // sides
            t.position = glm::vec3(max_pole_x, 0, t.position.z);
            root->addChild(std::make_shared<Entity>(pole_textured_model, t));
            t.position.x = -max_pole_x;
            root->addChild(std::make_shared<Entity>(pole_textured_model, t));
        }
    }
}

void SailingShip::generateMast() {
    const float MAST_POLE_DIAMETER = 0.5f;
    const float MAST_POLE_HEIGHT = 12.0f;
    Transform t;
    t.rotation.x += 90.0f;
    t.position.z -= 8.0f;
    root->addChild(ShapeFactory::createCylinder(t, WOOD_TEXTURE_PATH, material, MAST_POLE_DIAMETER,
                                                MAST_POLE_HEIGHT, 25));
    // mast
    const float MAST_HEIGHT = 8.0f;
    const float MAST_WIDTH = 6.0f;
    const float MAST_THICKNESS = 0.5f;
    const float MAST_ROTATION = 30.0f;
    const glm::vec3 MAST_OFFSET = glm::vec3(2.7f, 1.3f, -9.0f);
    ModelPtr mast_model = std::make_unique<Model>();
    Shape mast_shape = shape::cuboid(MAST_WIDTH, MAST_HEIGHT, MAST_THICKNESS);
    mast_model->load(mast_shape);
    auto linen_texture = TextureManager::getInstance().getTexture(LINEN_TEXTURE_PATH);
    TexturedModelSPtr mast_textured_model =
        std::make_shared<TexturedModel>(std::move(mast_model), linen_texture);
    t.reset();
    t.rotation = glm::vec3(90.0f, MAST_ROTATION, 0.0f);
    t.position = MAST_OFFSET;
    root->addChild(std::make_shared<Entity>(mast_textured_model, t));
    t.reset();
    t.rotation = glm::vec3(90.0f, -MAST_ROTATION, 0.0f);
    t.position = glm::vec3(-MAST_OFFSET.x + std::sin(glm::radians(-MAST_ROTATION) * MAST_WIDTH),
                           MAST_OFFSET.y, MAST_OFFSET.z);
    root->addChild(std::make_shared<Entity>(mast_textured_model, t));
}

void SailingShip::generateSteeringWheel() {
    // steering wheel platform
    const glm::vec3 PLATFORM_DIM = glm::vec3(2.0f, 2.0f, 2.0f);
    const glm::vec3 PLATFORM_OFFSET = glm::vec3(0, 4.0f, -3.5f);
    Transform t;
    t.position = PLATFORM_OFFSET;
    root->addChild(ShapeFactory::createCuboid(t, WOOD_TEXTURE_PATH, material, PLATFORM_DIM.x,
                                              PLATFORM_DIM.y, PLATFORM_DIM.z));
    // steering wheel
    t.rotation.x = 90.0f;
    t.position = PLATFORM_OFFSET + glm::vec3(0.0f, 1.2f, -0.5f);
    steering_wheel = ShapeFactory::createTorus(t, WOOD_TEXTURE_PATH, material, 1.0f, 0.1f, 30, 15);
    t.reset();
    t.rotation.x = 90.0f;
    t.position.z += 0.15f;
    steering_wheel->addChild(
        ShapeFactory::createCylinder(t, WOOD_TEXTURE_PATH, material, 0.25f, 0.50f, 25));
    const int NUM_OF_SPOKES = 8;
    const float ANGLE_STEP = 360.0f / NUM_OF_SPOKES;
    t.reset();
    for (int i = 0; i < NUM_OF_SPOKES / 2; i++) {
        t.rotation.z = ANGLE_STEP * i;
        steering_wheel->addChild(
            ShapeFactory::createCylinder(t, WOOD_TEXTURE_PATH, material, 0.12f, 2.0f, 25));
    }
    root->addChild(steering_wheel);
}

void SailingShip::generateTorches() {
    Transform torch_transform;
    torch_transform.rotation.x = 90.0f;
    torch_transform.position = glm::vec3(2, -7, -3);
    torches.push_back(Torch(torch_transform));
    lights.push_back(LightFactory::createPointLight(
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.01f, 0.1f));
    torch_transform.position.x -= 4.0f;
    torches.push_back(Torch(torch_transform));
    lights.push_back(LightFactory::createPointLight(
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.01f, 0.1f));
    for (const auto& torch : torches) {
        root->addChild(torch.getEntity());
    }
    for (const auto& light : lights) {
        LightManager::getInstance().addPointLight(light);
    }
}

// clang-format off
glm::vec3 point_from_transforms(const Transform &fst, const Transform &snd, const Transform &trd) {
    glm::mat4 transform(1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    createTransformMatrix(transform, fst);
    glm::mat4 transform2(1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    createTransformMatrix(transform2, snd);
    glm::mat4 transform3(1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    createTransformMatrix(transform3, trd);
    return glm::vec3((transform * transform2 * transform3)[3] / transform[3][3]);
}
// clang-format on

void SailingShip::animate(float delta_time) {
    static const float ANIM_SPEED = 0.001f;
    animation_time += delta_time;
    if (stationary) {
        steering_wheel->rotate(glm::vec3(0.0f, 0.0f, std::sin(animation_time * ANIM_SPEED)));
    } else {
        handleMovement(delta_time);
    }
    root->rotate(glm::vec3(0.0f, LEAN_MULT * std::cos(animation_time * ANIM_SPEED), 0.0f));
    for (int i = 0; i < lights.size(); ++i) {
        Transform t_root = root->getTransform();
        Transform t_torch = torches[i].getEntity()->getTransform();
        Transform t_fire = torches[i].getEntity()->getChildren()[0]->getTransform();
        lights[i]->position = point_from_transforms(t_root, t_torch, t_fire);
    }
}
void SailingShip::handleMovement(float delta_time) {
    static const auto rotate = [=](float rot_z) {
        getEntity()->move(-front * DECK_LENGTH / 2.0f);
        getEntity()->rotate(glm::vec3(0, 0, rot_z));
        getEntity()->move(front * DECK_LENGTH / 2.0f);
        front = utils::rotate_y(front, -rot_z);
        steering_wheel->rotate(glm::vec3(0.0f, 0.0f, rot_z));
    };
    DisplayManager& dm = DisplayManager::getInstance();
    glm::vec3 cur_pos = getEntity()->getPosition();
    if (dm.isKeyPressed(GLFW_KEY_W)) {
        getEntity()->setPosition(cur_pos + front * (-SHIP_SPEED) * delta_time);
    } else if (dm.isKeyPressed(GLFW_KEY_S)) {
        getEntity()->setPosition(cur_pos - front * (-SHIP_SPEED) * delta_time);
    }
    if (dm.isKeyPressed(GLFW_KEY_D)) {
        rotate(SHIP_STEERING_SPEED * delta_time);
    } else if (dm.isKeyPressed(GLFW_KEY_A)) {
        rotate(-SHIP_STEERING_SPEED * delta_time);
    }
}
