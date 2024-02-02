#include "cubemap.hpp"

Cubemap::Cubemap(ModelPtr model, TextureSPtr texture) : texture(texture), model(std::move(model)) {}

const TextureSPtr Cubemap::getTexture() const { return texture; }

void Cubemap::setTexture(TextureSPtr texture) { this->texture = texture; }