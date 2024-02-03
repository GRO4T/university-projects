#include "entity/complex/lighthouse.hpp"

#include "entity/shape_factory.hpp"

Lighthouse::Lighthouse(const Transform& t) {
    tower_texture = TextureManager::getInstance().getTexture(TOWER_TEXTURE_PATH);
    roof_texture = TextureManager::getInstance().getTexture(ROOF_TEXTURE_PATH);
    column_texture = TextureManager::getInstance().getTexture(COLUMNS_TEXTURE_PATH);
    lightbulb_texture = TextureManager::getInstance().getTexture(LIGHTHOUSE_TEXTURE_PATH);
    createTower(t);
    createRoof();
    createColumns();
    createLamp();
    createLightbulb(t);
}

void Lighthouse::animate(float delta_time) {
    const float ANIM_SPEED = 0.0005f;
    angle += ANIM_SPEED * delta_time;
    angle = fmod(angle, 360.0f);
    LightManager::getInstance().getSpotlights()[0]->updateDirection(
        glm::vec3(cos(angle), 0.0, sin(angle)));
}

void Lighthouse::createTower(const Transform& lighthouse_transform) {
    ModelPtr tower_model = std::make_unique<Model>();
    Shape tower_shape = shape::cylinder(6, 30, 25);
    tower_model->load(tower_shape);
    MaterialSPtr towerMaterial =
        std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(0.1f, 0.1f, 0.1f), 32.0f, 1.0f);
    TexturedModelSPtr tower_texture_model =
        std::make_shared<TexturedModel>(std::move(tower_model), tower_texture, towerMaterial);
    root = std::make_shared<Entity>(std::move(tower_texture_model), lighthouse_transform);
}
void Lighthouse::createRoof() {
    MaterialSPtr material =
        std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(0.1f, 0.1f, 0.1f), 32.0f, 1.0f);
    Transform t;
    t.position.y += 20.0f;
    root->addChild(ShapeFactory::createCone(t, ROOF_TEXTURE_PATH, material, 10, 4, 25));
}
void Lighthouse::createColumns() {
    float angle = 0.0f;
    ModelPtr model = std::make_unique<Model>();
    Shape shape = shape::cylinder(0.5, 4, 25);
    model->load(shape);
    MaterialSPtr material =
        std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(0.1f, 0.1f, 0.1f), 32.0f, 1.0f);
    TexturedModelSPtr textured_model =
        std::make_shared<TexturedModel>(std::move(model), column_texture, material);
    for (int i = 0; i < 6; ++i) {
        angle += 60.0f * 3.1415f / 180.f;
        Transform t;
        t.position.y += 16.0f;
        t.position.z += cos(angle) * 2.7f;
        t.position.x += sin(angle) * 2.7f;
        EntitySPtr column = std::make_shared<Entity>(textured_model, t);
        root->addChild(column);
    }
}
void Lighthouse::createLamp() {
    MaterialSPtr material =
        std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f), 32.0f, 1.0f);
    Transform t;
    t.position.y += 15.50f;
    root->addChild(ShapeFactory::createCylinder(t, COLUMNS_TEXTURE_PATH, material, 1, 1, 25));
}
void Lighthouse::createLightbulb(const Transform& lighthouse_transform) {
    ModelPtr model = std::make_unique<Model>();
    Shape shape = shape::sphere(2, 25);
    shape::invert_normals(shape);
    model->load(shape);
    MaterialSPtr material =
        std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f),
                                   glm::vec3(0.0f, 0.0f, 0.0f), 32.0f, 1.0f);
    TexturedModelSPtr textured_model =
        std::make_shared<TexturedModel>(std::move(model), lightbulb_texture, material);
    Transform t;
    t.position.y += 16.50f;
    EntitySPtr lightbulb = std::make_shared<Entity>(textured_model, t);
    root->addChild(lightbulb);

    LightManager::getInstance().addSpotlight(LightFactory::createSpotLight(
        lighthouse_transform.position + glm::vec3(0.0f, 16.5f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
        45.0f, 60.0f, 0.5f, 0.001f, 0.0001f));
}
