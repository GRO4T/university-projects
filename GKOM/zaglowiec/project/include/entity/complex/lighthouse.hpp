#pragma once

#include "complex_entity.hpp"
#include "lighting/light_factory.hpp"
#include "lighting/light_manager.hpp"

class Lighthouse : public ComplexEntity {
public:
    Lighthouse(const Transform& t);

    void animate(float delta_time) override;

private:
    void createTower(const Transform& lighthouse_transform);
    void createRoof();
    void createColumns();
    void createLamp();
    void createLightbulb(const Transform& lighthouse_transform);

    const std::string TOWER_TEXTURE_PATH = "res/lighthouse.jpg";
    const std::string ROOF_TEXTURE_PATH = "res/roof.jpg";
    const std::string COLUMNS_TEXTURE_PATH = "res/columns.jpg";
    const std::string LIGHTHOUSE_TEXTURE_PATH = "res/lightbulb.jpg";

    float angle = 0;

    TextureSPtr tower_texture;
    TextureSPtr roof_texture;
    TextureSPtr column_texture;
    TextureSPtr lightbulb_texture;
};