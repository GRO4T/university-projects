#pragma once

#include <vector>

#include "model/model.hpp"
#include "texture/texture.hpp"

class Cubemap {
public:
    Cubemap(ModelPtr model, TextureSPtr texture);
    const Model& getModel() const { return *model; }
    const TextureSPtr getTexture() const;
    void setTexture(TextureSPtr texture);
private:
    ModelPtr model;
    TextureSPtr texture;
};

using CubemapSharedPtr = std::shared_ptr<Cubemap>;