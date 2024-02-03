#include "terrain.hpp"

#include "stb_image.h"
#include "utils.hpp"

const MaterialSPtr Terrain::DEFAULT_MATERIAL =
    std::make_shared<Material>(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.7f, 0.7f, 0.7f),
                               glm::vec3(0.1f, 0.1f, 0.1f), 32.0f, 0.5f);

Terrain::Terrain(const glm::vec3& pos, TextureSPtr texture, const std::string& height_map,
                 float max_height, MaterialSPtr material)
    : texture(texture), max_height(max_height), material(material) {
    x = pos.x * SIZE;
    z = pos.z * SIZE;
    base_height = pos.y;
    generate(height_map);
}

void Terrain::generate(const std::string& height_map_path) {  // this should also calculate normals
    utils::Image height_map;
    height_map.load(height_map_path);

    // we assume that our terrain is a square
    int vertices_per_side = height_map.getWidth();
    int vertex_count = vertices_per_side * vertices_per_side;
    std::vector<GLfloat> vertices(vertex_count * 3);
    std::vector<GLfloat> normals(vertex_count * 3);
    std::vector<GLfloat> tex_coords(vertex_count * 2);
    std::vector<GLuint> indices(6 * (vertices_per_side - 1) * vertices_per_side);
    int v = 0;
    for (int i = 0; i < vertices_per_side; ++i) {
        for (int j = 0; j < vertices_per_side; ++j) {
            vertices[v * 3] = (GLfloat)j / ((GLfloat)vertices_per_side - 1) * SIZE;
            vertices[v * 3 + 1] = base_height + getHeight(j, i, height_map);
            vertices[v * 3 + 2] = (GLfloat)i / ((GLfloat)vertices_per_side - 1) * SIZE;
            glm::vec3 normal = calculateNormal(j, i, height_map);
            normals[v * 3] = normal.x;
            normals[v * 3 + 1] = normal.y;
            normals[v * 3 + 2] = normal.z;
            tex_coords[v * 2] = (GLfloat)j / ((GLfloat)vertices_per_side - 1);
            tex_coords[v * 2 + 1] = (GLfloat)i / ((GLfloat)vertices_per_side - 1);
            v++;
        }
    }
    v = 0;
    for (int gz = 0; gz < vertices_per_side - 1; ++gz) {
        for (int gx = 0; gx < vertices_per_side - 1; ++gx) {
            int top_left = (gz * vertices_per_side) + gx;
            int top_right = top_left + 1;
            int bottom_left = ((gz + 1) * vertices_per_side) + gx;
            int bottom_right = bottom_left + 1;
            indices[v++] = top_left;
            indices[v++] = bottom_left;
            indices[v++] = top_right;
            indices[v++] = top_right;
            indices[v++] = bottom_left;
            indices[v++] = bottom_right;
        }
    }

    model.load(vertices, normals, tex_coords, indices);
}

float Terrain::getHeight(int x, int z, const utils::Image& height_map) {
    if (x < 0 || x >= height_map.getWidth() || z < 0 || z >= height_map.getHeight()) {
        return 0;
    }
    float height = height_map.getRGB(x, z);
    height -= MAX_PIXEL_COLOR / 2.0f;
    height /= MAX_PIXEL_COLOR / 2.0f;
    height *= max_height;
    return height;
}

glm::vec3 Terrain::calculateNormal(int x, int z, const utils::Image& height_map) {
    float height_l = base_height + getHeight(x - 1, z, height_map);
    float height_r = base_height + getHeight(x + 1, z, height_map);
    float height_d = base_height + getHeight(x, z - 1, height_map);
    float height_u = base_height + getHeight(x, z + 1, height_map);
    glm::vec3 normal = glm::vec3(height_l - height_r, 2.0f, height_d - height_u);
    normal = glm::normalize(normal);
    return normal;
}
