#include "water.hpp"

#include "model/shape.hpp"

Water::Water(const glm::vec3 &root_, float size_, float segment_size_, float wave_size_,
             Material material_)
    : root(root_), wave_size(wave_size_), material(material_) {
    model.load(shape::plane(size_, size_, static_cast<unsigned int>(size_ / segment_size_),
                            static_cast<unsigned int>(size_ / segment_size_)));
}

utils::Transform Water::getTransform() const {
    return utils::Transform(root, glm::vec3(-90, 0, 0), glm::vec3(1, 1, 1));
}
