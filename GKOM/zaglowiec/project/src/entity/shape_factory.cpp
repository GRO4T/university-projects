#include "entity/shape_factory.hpp"

#include "texture/texture_manager.hpp"

EntitySPtr ShapeFactory::createShape(ModelPtr model, const Transform& t,
                                     const std::string& texture_path, MaterialSPtr material) {
    TextureSPtr texture = TextureManager::getInstance().getTexture(texture_path);
    TexturedModelSPtr textured_model = std::make_shared<TexturedModel>(std::move(model), texture);
    EntitySPtr entity = std::make_shared<Entity>(textured_model, t);
    return entity;
}

EntitySPtr ShapeFactory::createCube(const Transform& t, const std::string& tex_path,
                                    MaterialSPtr material) {
    ModelPtr model = std::make_unique<Model>();
    Shape model_shape = shape::cube();
    model->load(model_shape);
    return createShape(std::move(model), t, tex_path, material);
}

EntitySPtr ShapeFactory::createCuboid(const Transform& t, const std::string& tex_path,
                                      MaterialSPtr material, GLfloat width, GLfloat height,
                                      GLfloat depth) {
    ModelPtr model = std::make_unique<Model>();
    Shape model_shape = shape::cuboid(width, height, depth);
    model->load(model_shape);
    return createShape(std::move(model), t, tex_path, material);
}

EntitySPtr ShapeFactory::createCylinder(const Transform& t, const std::string& tex_path,
                                        MaterialSPtr material, GLfloat diameter, GLfloat height,
                                        uint32_t sides) {
    ModelPtr model = std::make_unique<Model>();
    Shape model_shape = shape::cylinder(diameter, height, sides);
    model->load(model_shape);
    return createShape(std::move(model), t, tex_path, material);
}

EntitySPtr ShapeFactory::createCone(const Transform& t, const std::string& tex_path,
                                    MaterialSPtr material, GLfloat diameter, GLfloat height,
                                    uint32_t sides) {
    ModelPtr model = std::make_unique<Model>();
    Shape model_shape = shape::cone(diameter, height, sides);
    model->load(model_shape);
    return createShape(std::move(model), t, tex_path, material);
}

EntitySPtr ShapeFactory::createSphere(const Transform& t, const std::string& tex_path,
                                      MaterialSPtr material, GLfloat diameter, uint32_t sides) {
    ModelPtr model = std::make_unique<Model>();
    Shape model_shape = shape::sphere(diameter, sides);
    model->load(model_shape);
    return createShape(std::move(model), t, tex_path, material);
}

CubemapSharedPtr ShapeFactory::createCubemap(const std::vector<std::string>& tex_paths) {
    ModelPtr model = std::make_unique<Model>();
    Shape model_shape = shape::cubemap();
    model->load(model_shape);
    TextureSPtr texture =
        TextureManager::getInstance().getLoadCubemapTexture(tex_paths[0], tex_paths);
    CubemapSharedPtr cubemap = std::make_shared<Cubemap>(std::move(model), texture);
    return cubemap;
}

EntitySPtr ShapeFactory::createTorus(const Transform& t, const std::string& tex_path,
                                     MaterialSPtr material, GLfloat radius, GLfloat cs_radius,
                                     unsigned int sides, unsigned int cs_sides) {
    /* based on: https://gamedev.stackexchange.com/questions/16845/how-do-i-generate-a-torus-mesh */
    int num_vertices = (sides + 1) * (cs_sides + 1);
    int num_indices = (2 * sides + 4) * cs_sides;
    std::vector<GLfloat> vertices(3 * num_vertices);
    std::vector<GLfloat> normals(3 * num_vertices);
    std::vector<GLfloat> tex_coords(2 * num_vertices);
    std::vector<GLuint> indices(num_indices);

    int angleincs = 360 / sides;
    int cs_angleincs = 360 / cs_sides;
    float current_radius, zval;
    int i, j, next_row;

    /* iterate cs_sides: inner ring */
    int vertex_id = 0;
    int tex_coord_id = 0;
    for (j = 0; j <= 360; j += cs_angleincs) {
        current_radius = radius + (cs_radius * std::cos(glm::radians(float(j))));
        zval = cs_radius * std::sin(glm::radians(float(j)));

        /* iterate sides: outer ring */
        for (i = 0; i <= 360; i += angleincs, vertex_id += 3, tex_coord_id += 2) {
            vertices[vertex_id] = current_radius * std::cos(glm::radians(float(i)));
            vertices[vertex_id + 1] = current_radius * std::sin(glm::radians(float(i)));
            vertices[vertex_id + 2] = zval;

            float u = i / 360.0f;
            float v = 2.0f * j / 360.0f - 1.0f;
            if (v < 0) v = -v;

            tex_coords[tex_coord_id] = u;
            tex_coords[tex_coord_id + 1] = v;
        }
    }

    /* compute normals: loops are swapped */
    int vertex_id_outer = 0;
    int normal_id_outer = 0;
    for (i = 0, next_row = (sides + 1) * 3; i <= 360;
         i += angleincs, normal_id_outer += 3, vertex_id_outer += 3) {
        float xc = radius * cos(glm::radians(float(i)));
        float yc = radius * sin(glm::radians(float(i)));
        int vertex_id = vertex_id_outer;
        int normal_id = normal_id_outer;
        for (j = 0; j <= 360; j += cs_angleincs, vertex_id += next_row, normal_id += next_row) {
            glm::vec3 normals_copy(vertices[vertex_id] - xc, vertices[vertex_id + 1] - yc,
                                   vertices[vertex_id + 2]);
            normals_copy = glm::normalize(normals_copy);
            normals[normal_id] = normals_copy.x;
            normals[normal_id + 1] = normals_copy.y;
            normals[normal_id + 2] = normals_copy.z;
        }
    }

    /* indices grouped by GL_TRIANGLE_STRIP, face oriented clock-wise */

    /* inner ring */
    int index_id = 0;
    for (i = 0, next_row = sides + 1; i < static_cast<int>(cs_sides); i++) {
        /* outer ring */
        for (j = 0; j < static_cast<int>(sides); j++) {
            indices[index_id++] = i * next_row + j;
            indices[index_id++] = (i + 1) * next_row + j;
        }

        /* generate dummy triangle to avoid messing next ring */
        indices[index_id] = i * next_row + j;
        indices[index_id + 1] = indices[index_id + 2] = indices[index_id + 3] =
            indices[index_id] + next_row;
        index_id += 4;
    }

    StripedModelPtr model = std::make_unique<StripedModel>();
    model->load(vertices, normals, tex_coords, indices);
    return createShape(std::move(model), t, tex_path, material);
}
