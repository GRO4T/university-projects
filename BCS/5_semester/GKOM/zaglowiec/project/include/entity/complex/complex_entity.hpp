#pragma once

#include "entity/entity.hpp"
#include "model/shape.hpp"
#include "texture/texture_manager.hpp"

class ComplexEntity {
public:
    EntitySPtr getEntity() const { return root; }
    virtual void animate(float delta_time) {};
protected:
    EntitySPtr root;
    float animation_time = 0.0f;
};