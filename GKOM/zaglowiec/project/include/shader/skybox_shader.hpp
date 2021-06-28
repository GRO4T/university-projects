#pragma once

#include "shader_program.hpp"

class SkyboxShader : public ShaderProgram {
public:
    SkyboxShader()
        : ShaderProgram("res/shaders/skybox_shader.vert", "res/shaders/skybox_shader.frag") {}
    virtual ~SkyboxShader() {}
};