#pragma once

#include "shader_program.hpp"

class EntityShader : public ShaderProgram {
public:
    EntityShader()
        : ShaderProgram("res/shaders/entity_shader.vert", "res/shaders/entity_shader.frag") {}
    virtual ~EntityShader() {}
};