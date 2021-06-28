#include "utils.hpp"

#include <algorithm>
#include <stdexcept>

#include "entity/entity.hpp"
#include "stb_image.h"

namespace utils {
Loader &Loader::getInstance() {
    static Loader instance;
    return instance;
}

Loader::Loader() {
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                                 // basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Loader::loadMipmapTexture(GLuint texture_id, const char *filename) {
    // int width, height;
    // unsigned char *image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
    int width, height, nrChannels;
    unsigned char *image = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (image == nullptr) throw std::runtime_error("Failed to load texture file");

    GLuint texture;
    glGenTextures(1, &texture);

    glActiveTexture(texture_id);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1);  // this line fixes random seqfaults
             // https://stackoverflow.com/questions/9950546/c-opengl-glteximage2d-access-violation
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // SOIL_free_image_data(image);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint Loader::loadCubemap(const std::vector<std::string> &faces) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

void createViewMatrix(glm::mat4 &view, const Camera &camera) {
    view = glm::mat4(1.0f);
    view = glm::rotate(view, (float)glm::radians(camera.getPitch()), glm::vec3(1, 0, 0));
    view = glm::rotate(view, (float)glm::radians(camera.getYaw()), glm::vec3(0, 1, 0));
    view = glm::translate(view, -camera.getPosition());
}

void createTransformMatrix(glm::mat4 &transformation_matrix, const Transform &object_transform) {
    transformation_matrix = glm::mat4(1.0f);
    const glm::vec3 &pos = object_transform.position;
    transformation_matrix = glm::translate(transformation_matrix, pos);
    const glm::vec3 &rot = object_transform.rotation;
    transformation_matrix =
        glm::rotate(transformation_matrix, (float)glm::radians(rot.x), glm::vec3(1, 0, 0));
    transformation_matrix =
        glm::rotate(transformation_matrix, (float)glm::radians(rot.y), glm::vec3(0, 1, 0));
    transformation_matrix =
        glm::rotate(transformation_matrix, (float)glm::radians(rot.z), glm::vec3(0, 0, 1));
    const glm::vec3 &scale = object_transform.scale;
    transformation_matrix = glm::scale(transformation_matrix, scale);
}

std::ostream &operator<<(std::ostream &os, const glm::mat4 &mx) {
    for (int Row = 0; Row < 4; ++Row) {
        for (int Col = 0; Col < 4; ++Col) std::cout << mx[Row][Col] << ' ';
        std::cout << std::endl;
    }
    return os;
}

void Image::load(const std::string &path) {
    unsigned char *image = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
    if (image == nullptr) throw std::runtime_error("Failed to load texture file");
    std::unique_ptr<unsigned char[]> tmp(image);
    data = std::move(tmp);
}

glm::vec3 vec3_lerp(const glm::vec3 &a, const glm::vec3 &b, float ratio) {
    ratio = std::max(std::min(ratio, 1.0f), 0.0f);
    float rat_a = 1 - ratio;
    return glm::vec3(a.x * rat_a + b.x * ratio, a.y * rat_a + b.y * ratio,
                     a.z * rat_a + b.z * ratio);
}

glm::vec3 rotate_x(const glm::vec3 &v, float angle) {
    float sin = std::sin(glm::radians(angle));
    float cos = std::cos(glm::radians(angle));
    float ty = v.y;
    float tz = v.z;
    return glm::vec3(v.x, (cos * ty) - (sin * tz), (cos * tz) + (sin * ty));
}

glm::vec3 rotate_y(const glm::vec3 &v, float angle) {
    float sin = std::sin(glm::radians(angle));
    float cos = std::cos(glm::radians(angle));
    float tx = v.x;
    float tz = v.z;
    return glm::vec3((cos * tx) + (sin * tz), v.y, (cos * tz) - (sin * tx));
}

glm::vec3 rotate_z(const glm::vec3 &v, float angle) {
    float sin = std::sin(glm::radians(angle));
    float cos = std::cos(glm::radians(angle));
    float tx = v.x;
    float ty = v.y;
    return glm::vec3((cos * tx) - (sin * ty), (cos * ty) + (sin * tx), v.z);
}

}  // namespace utils
