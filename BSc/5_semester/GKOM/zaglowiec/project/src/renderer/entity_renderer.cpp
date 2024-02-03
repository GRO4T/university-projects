#include "renderer/entity_renderer.hpp"

#include "demo.hpp"
#include "utils.hpp"

using utils::Transform;

EntityRenderer::EntityRenderer(const glm::mat4& projection_matrix) {
    shader.use();
    shader.setMat4("Projection", projection_matrix);
    shader.setInt("Texture0", 0);
    shader.setInt("Texture1", 1);
}

void EntityRenderer::render(std::vector<EntitySPtr>& entities, const Camera& camera) {
    light_pos = (LightManager::getInstance().getSpotlights().front())->position;
    ship_pos = shadowEntity->getPosition();
    light_pos = ship_pos - 40.f * glm::normalize(ship_pos - light_pos);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;

    float near_plane = 10.f, far_plane = 70.f;
    lightProjection = glm::ortho(-30.0f, 20.0f, -30.0f, 20.0f, near_plane, far_plane);
    lightView = glm::lookAt(light_pos, ship_pos, glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    shadowShader.use();
    shadowShader.setMat4("LightSpace", lightSpaceMatrix);

    glViewport(0, 0, TextureManager::getInstance().SHADOW_WIDTH,
               TextureManager::getInstance().SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, TextureManager::getInstance().depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE1);
    for (auto& entity : entities) {
        Model* model = entity->getTexturedModel().getModel();
        glm::mat4 transform_matrix;
        utils::createTransformMatrix(transform_matrix, entity->getTransform());
        shadowShader.setMat4("Transform", transform_matrix);
        glBindVertexArray(model->getVao());
        model->draw();
        unbind(*model);
        renderChildrenShadows(*entity, transform_matrix, lightSpaceMatrix);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const GLuint WIDTH = 1440, HEIGHT = 900;
    glViewport(0, 0, WIDTH, HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setViewMatrix(camera);
    LightManager::getInstance().setShader(shader);

    for (auto& entity : entities) {
        Model* model = entity->getTexturedModel().getModel();
        glm::mat4 transform_matrix;
        utils::createTransformMatrix(transform_matrix, entity->getTransform());
        shader.setMat4("Transform", transform_matrix);
        shader.setVec3("CameraPosition", camera.getPosition());
        prepareInstance(*entity);
        shader.setVec3("lightPosition", light_pos);
        shader.setMat4("LightSpace", lightSpaceMatrix);
        model->draw();
        unbind(*model);
        renderChildren(*entity, transform_matrix);
    }
}

void EntityRenderer::renderChildren(Entity& parent, glm::mat4 combined_transform) {
    for (auto& child : parent.getChildren()) {
        Entity& entity = (Entity&)*child;
        glm::mat4 transform_matrix;
        utils::createTransformMatrix(transform_matrix, entity.getTransform());
        shader.setMat4("Transform", combined_transform * transform_matrix);
        prepareInstance(entity);
        entity.getTexturedModel().getModel()->draw();
        unbind(*entity.getTexturedModel().getModel());
        renderChildren(entity, combined_transform * transform_matrix);
    }
}

void EntityRenderer::renderChildrenShadows(Entity& parent, glm::mat4 combined_transform,
                                           glm::mat4 lightSpaceMatrix) {
    for (auto& child : parent.getChildren()) {
        Entity& entity = (Entity&)*child;
        glm::mat4 transform_matrix;
        utils::createTransformMatrix(transform_matrix, entity.getTransform());
        shadowShader.setMat4("Transform", combined_transform * transform_matrix);
        shadowShader.setMat4("LightSpace", lightSpaceMatrix);
        glBindVertexArray(entity.getTexturedModel().getModel()->getVao());
        entity.getTexturedModel().getModel()->draw();
        unbind(*entity.getTexturedModel().getModel());
        renderChildrenShadows(entity, combined_transform * transform_matrix, lightSpaceMatrix);
    }
}

void EntityRenderer::unbind(const Model& model) { glBindVertexArray(0); }

void EntityRenderer::prepareInstance(const Entity& entity) {
    auto& textured_model = entity.getTexturedModel();
    const std::vector<TextureSPtr>& textures = textured_model.getAllTextures();
    glBindVertexArray(textured_model.getModel()->getVao());
    for (auto& texture : textures) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getTexture());
        glUniform1i(
            glGetUniformLocation(shader.getProgramID(), texture->getTextureUnitShaderName()),
            texture->getTextureUnitShaderID());
    }
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, TextureManager::getInstance().depthMap);
    shader.setVec3("material.ambient", textured_model.getMaterial().ambient);
    shader.setVec3("material.diffuse", textured_model.getMaterial().diffuse);
    shader.setVec3("material.specular", textured_model.getMaterial().specular);
    shader.setFloat("material.shininess", textured_model.getMaterial().shininess);
    shader.setFloat("material.alpha", textured_model.getMaterial().alpha);
}

void EntityRenderer::prepareInstanceShadows(const Entity& entity) {
    auto& textured_model = entity.getTexturedModel();
    glBindVertexArray(textured_model.getModel()->getVao());
}
