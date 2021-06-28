#include "renderer/water_renderer.hpp"

#include "lighting/light_manager.hpp"

WaterRenderer::WaterRenderer(const glm::mat4& projection_matrix) {
    shader.use();
    shader.setMat4("Projection", projection_matrix);
}

void WaterRenderer::render(WaterSharedPtr& water, const Camera& camera, float time) {
    shader.use();
    shader.setViewMatrix(camera);

    shader.setVec3("material.ambient", water->getMaterial().ambient);
    shader.setVec3("material.diffuse", water->getMaterial().diffuse);
    shader.setVec3("material.specular", water->getMaterial().specular);
    shader.setFloat("material.shininess", water->getMaterial().shininess);
    shader.setFloat("material.alpha", water->getMaterial().alpha);

    LightManager::getInstance().setShader(shader);

    glm::mat4 transform_matrix;
    utils::createTransformMatrix(transform_matrix, water->getTransform());
    shader.setMat4("Transform", transform_matrix);

    glm::vec3 cp = camera.getPosition();
    shader.setVec4("CameraPosition", glm::vec4(cp.x, cp.y, cp.z, 1.0));

    shader.setFloat("Time", time);

    shader.setFloat("WaveSize", water->getWaveSize());

    glBindVertexArray(water->getModel().getVao());

    glDrawArrays(GL_TRIANGLES, 0, water->getModel().getVertexCount());

    glBindVertexArray(0);
}
