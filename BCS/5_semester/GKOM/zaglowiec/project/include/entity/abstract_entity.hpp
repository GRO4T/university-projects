#pragma once

#include <memory>

#include "model/textured_model.hpp"
#include "utils.hpp"

using utils::Transform;

class AbstractEntity {
public:
    virtual const TexturedModel& getTexturedModel() const = 0;

    virtual void setTransform(const Transform& transform) = 0;
    virtual const Transform& getTransform() const = 0;

    virtual void setPosition(const glm::vec3& pos) = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void move(const glm::vec3& translation) = 0;

    virtual void setRotation(const glm::vec3& rot) = 0;
    virtual const glm::vec3& getRotation() const = 0;
    virtual void rotate(const glm::vec3& rot) = 0;

    virtual void setScale(const glm::vec3& scale) = 0;
    virtual const glm::vec3& getScale() const = 0;
    virtual ~AbstractEntity() {}
};

using AbstractEntityPtr = std::unique_ptr<AbstractEntity>;
using AbstractEntitySharedPtr = std::shared_ptr<AbstractEntity>;