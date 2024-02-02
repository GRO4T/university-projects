#pragma once

#include <iostream>
#include <vector>

#include "model/material.hpp"
#include "model/model.hpp"
#include "texture/texture.hpp"

class TexturedModel {
public:
    TexturedModel(ModelPtr model, TextureSPtr texture,
                  MaterialSPtr material = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f),
                                                                     glm::vec3(1.0f, 1.0f, 1.0f),
                                                                     glm::vec3(0.5f, 0.5f, 0.5f),
                                                                     32.0f, 1.0f))
        : model(std::move(model)), material(material) {
        textures.push_back(texture);
    }
    TexturedModel(ModelPtr model, MaterialSPtr material = std::make_shared<Material>(
                                      glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                      glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, 1.0f))
        : model(std::move(model)), material(material) {}
    ~TexturedModel() {}
    Model* getModel() { return model.get(); }
    const std::vector<TextureSPtr>& getAllTextures() const { return textures; }
    void addTexture(TextureSPtr texture) { textures.push_back(texture); }
    Material getMaterial() const { return *material; }

private:
    ModelPtr model;
    std::vector<TextureSPtr> textures;
    MaterialSPtr material;
};

using TexturedModelPtr = std::unique_ptr<TexturedModel>;
using TexturedModelSPtr = std::shared_ptr<TexturedModel>;
