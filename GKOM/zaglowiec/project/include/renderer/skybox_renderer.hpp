#pragma once

#include <vector>

#include "cubemap.hpp"
#include "shader/skybox_shader.hpp"

class SkyboxRenderer {
public:
    SkyboxRenderer(const glm::mat4& projection_matrix);
    void render(CubemapSharedPtr skybox, const Camera& camera);

private:
    void prepareInstance(const Cubemap& skybox);
    SkyboxShader shader;
};