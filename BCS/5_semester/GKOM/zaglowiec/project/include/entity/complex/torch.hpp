#pragma once

#include "complex_entity.hpp"
#include "entity/shape_factory.hpp"

class Torch : public ComplexEntity {
public:
    Torch(const Transform& t) {
        wood_texture = TextureManager::getInstance().getTexture(WOOD_TEXTURE_PATH);
        material =
            std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                       glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, 1.0f);
        root = ShapeFactory::createCuboid(t, WOOD_TEXTURE_PATH, material, WIDTH, HEIGHT, WIDTH);
        createFire();
    }
    void animate(float delta_time) override {}

private:
    const std::string WOOD_TEXTURE_PATH = "res/wood.jpg";
    const float WIDTH = 0.17f;
    const float HEIGHT = 1.5f;

    void createFire() {
        ModelPtr model = std::make_unique<Model>();
        Shape shape = shape::cuboid(WIDTH, WIDTH, WIDTH);
        shape::invert_normals(shape);
        model->load(shape);
        TexturedModelSPtr textured_model = std::make_shared<TexturedModel>(
            std::move(model), wood_texture,
            std::make_shared<Material>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 30.0f, 30.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f), 32.0f, 1.0f));
        Transform t;
        t.position.y -= (HEIGHT + WIDTH) / 2;
        root->addChild(std::make_shared<Entity>(textured_model, t));
    }

    TextureSPtr wood_texture;
    MaterialSPtr material;
};
