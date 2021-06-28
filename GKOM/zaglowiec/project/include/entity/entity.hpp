#pragma once

#include <vector>

#include "entity/abstract_entity.hpp"
#include "utils.hpp"

using utils::Transform;

class Entity : public AbstractEntity {
public:
    Entity() {}
    Entity(TexturedModelSPtr textured_model, const Transform& transform)
        : textured_model(textured_model), transform(transform) {}
    Entity(const Entity&) = delete;
    Entity(Entity&& e) = delete;  // we don't use it right now
    /*
    {
        textured_model = e.textured_model;
        e.textured_model.reset();
        transform = e.transform;
    }
     */
    virtual ~Entity() {}

    TexturedModel& getTexturedModel() const override { return *textured_model; }

    void setTransform(const Transform& transform) override { this->transform = transform; }
    const Transform& getTransform() const override { return transform; }

    void setPosition(const glm::vec3& pos) override { transform.position = pos; }
    const glm::vec3& getPosition() const override { return transform.position; }
    void move(const glm::vec3& translation) override { transform.position += translation; }

    void setRotation(const glm::vec3& rot) override { transform.rotation = rot; }
    const glm::vec3& getRotation() const override { return transform.rotation; }
    void rotate(const glm::vec3& rot) override { transform.rotation += rot; }

    void setScale(const glm::vec3& scale) override { transform.scale = scale; }
    const glm::vec3& getScale() const override { return transform.scale; }

    void addChild(AbstractEntitySharedPtr child) { children.push_back(child); }
    std::vector<AbstractEntitySharedPtr>& getChildren() { return children; }

private:
    Transform transform;
    TexturedModelSPtr textured_model;

    std::vector<AbstractEntitySharedPtr> children;
};

using EntityPtr = std::unique_ptr<Entity>;
using EntitySPtr = std::shared_ptr<Entity>;
