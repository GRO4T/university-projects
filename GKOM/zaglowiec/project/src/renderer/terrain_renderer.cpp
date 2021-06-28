#include "renderer/terrain_renderer.hpp"

#include "lighting/light_manager.hpp"

TerrainRenderer::TerrainRenderer(const glm::mat4& projection_matrix) {
    shader.use();
    shader.setMat4("Projection", projection_matrix);
}

void TerrainRenderer::render(std::vector<TerrainSharedPtr>& terrains, const Camera& camera) {
    shader.use();
    shader.setViewMatrix(camera);
    LightManager::getInstance().setShader(shader);

    for (auto& terrain : terrains) {
        const Model& model = terrain->getModel();
        glm::mat4 transform_matrix;
        utils::Transform terrain_transform(glm::vec3(terrain->getX(), 0, terrain->getZ()),
                                           glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        utils::createTransformMatrix(transform_matrix, terrain_transform);
        shader.setMat4("Transform", transform_matrix);
        shader.setVec3("CameraPosition", camera.getPosition());
        prepareInstance(*terrain);
        terrain->getModel().draw();
        unbind(model);
    }
}
void TerrainRenderer::unbind(const Model& model) {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void TerrainRenderer::prepareInstance(const Terrain& terrain) {
    glBindVertexArray(terrain.getModel().getVao());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrain.getTexture().getTexture());
    glUniform1i(glGetUniformLocation(shader.getProgramID(), "Texture0"), 0);
    shader.setVec3("material.ambient", terrain.getMaterial().ambient);
    shader.setVec3("material.diffuse", terrain.getMaterial().diffuse);
    shader.setVec3("material.specular", terrain.getMaterial().specular);
    shader.setFloat("material.shininess", terrain.getMaterial().shininess);
    shader.setFloat("material.alpha", terrain.getMaterial().alpha);
}
