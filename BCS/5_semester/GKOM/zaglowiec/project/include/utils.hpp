#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "camera.hpp"
#include "opengl_all.hpp"
#include "stb_image.h"

class Entity;

namespace utils {

class Loader {
public:
    static Loader &getInstance();
    static GLuint loadMipmapTexture(GLuint texture_id, const char *filename);
    static GLuint loadCubemap(const std::vector<std::string> &faces);

private:
    Loader();
    Loader(const Loader &) = delete;
    Loader &operator=(const Loader &) = delete;
};

struct Transform {
    Transform() { reset(); }
    Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale) {
        position = pos;
        rotation = rot;
        this->scale = scale;
    }
    void reset() {
        position = rotation = glm::vec3(0, 0, 0);
        scale = glm::vec3(1, 1, 1);
    }
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

class Image {
public:
    void load(const std::string &path);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getNrChannels() const { return nr_channels; }
    unsigned char *getData() const { return data.get(); }
    float getRGB(int x, int y) const {
        unsigned bytes_per_pixel = nr_channels;
        unsigned char *pixel_offset = data.get() + (y * width + x) * bytes_per_pixel;
        unsigned char r = pixel_offset[0];
        unsigned char g = pixel_offset[1];
        unsigned char b = pixel_offset[2];
        return (float)(r * g * b);
    }

private:
    int width;
    int height;
    int nr_channels;
    std::unique_ptr<unsigned char[]> data;
};

void createViewMatrix(glm::mat4 &view, const Camera &camera);
void createTransformMatrix(glm::mat4 &transformation_matrix, const Transform &object_transform);

std::ostream &operator<<(std::ostream &os, const glm::mat4 &mx);

glm::vec3 vec3_lerp(const glm::vec3 &a, const glm::vec3 &b, float ratio);

glm::vec3 rotate_x(const glm::vec3 &v, float angle);
glm::vec3 rotate_y(const glm::vec3 &v, float angle);
glm::vec3 rotate_z(const glm::vec3 &v, float angle);
}  // namespace utils