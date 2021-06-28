#pragma once

#include "shader/shader_program.hpp"

class WaterShader : public ShaderProgram {
public:
    WaterShader()
        : ShaderProgram("res/shaders/water_shader.vert", "res/shaders/water_shader.frag") {}
    virtual ~WaterShader() {}
};