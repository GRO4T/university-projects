#include "renderer/skybox_renderer.hpp"

SkyboxRenderer::SkyboxRenderer(const glm::mat4& projection_matrix) {
    shader.use();
    shader.setMat4("projection", projection_matrix);
}

void SkyboxRenderer::render(CubemapSharedPtr skybox, const Camera& camera) {
    shader.use();
    shader.setViewMatrix(camera);

    glm::mat4 view;
    utils::createViewMatrix(view, camera);
    view = glm::mat4(glm::mat3(view));

    glDepthFunc(GL_LEQUAL);
    shader.use();
    shader.setMat4("view", view);

    prepareInstance(*skybox);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void SkyboxRenderer::prepareInstance(const Cubemap& skybox) {
    const TextureSPtr texture = skybox.getTexture();
    glBindVertexArray(skybox.getModel().getVao());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getTexture());
}