#pragma once

#include <vector>

#include "entity/entity.hpp"
#include "lighting/lighting.hpp"
#include "shader/entity_shader.hpp"
#include "shader/shadowmap_shader.hpp"

class EntityRenderer {
public:
    EntityRenderer(const glm::mat4& projection_matrix);
    void render(std::vector<EntitySPtr>& entities, const Camera& camera);
    void registerForShadows(EntitySPtr entity) { shadowEntity = entity; }

private:
    void unbind(const Model& model);
    void prepareInstance(const Entity& entity);
    void prepareInstanceShadows(const Entity& entity);
    void renderChildren(Entity& parent, glm::mat4 combined_transform);
    void renderChildrenShadows(Entity& parent, glm::mat4 combined_transform,
                               glm::mat4 lightSpaceMatrix);

    EntityShader shader;
    ShadowmapShader shadowShader;

    glm::vec3 light_pos;
    glm::vec3 ship_pos;

    EntitySPtr shadowEntity;
};
