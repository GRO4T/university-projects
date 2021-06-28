#pragma once

#include "shader/shader_program.hpp"

class TerrainShader : public ShaderProgram {
public:
    TerrainShader()
        : ShaderProgram("res/shaders/terrain_shader.vert", "res/shaders/entity_shader.frag") {
        bindAttributes();
    }
    ~TerrainShader() override {}

protected:
    void bindAttributes() const {
        bindAttribute(0, "Position");
        bindAttribute(1, "TexCoords");
        bindAttribute(2, "Normal");
    }
};