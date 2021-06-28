#pragma once

#include "shader_program.hpp"

class ShadowmapShader : public ShaderProgram {
public:
    ShadowmapShader()
        : ShaderProgram("res/shaders/shadowmap_shader.vert", "res/shaders/shadowmap_shader.frag") {}
    virtual ~ShadowmapShader() {}
};