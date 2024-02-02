#pragma once

#include <map>
#include <memory>
#include <string>

#include "opengl_all.hpp"
#include "utils.hpp"

class Texture {
public:
    Texture(GLuint texture_unit, const std::string& texture_path) : texture_unit(texture_unit) {
        texture =
            utils::Loader::getInstance().loadMipmapTexture(texture_unit, texture_path.c_str());
    }
    Texture(GLuint texture_unit, const std::vector<std::string>& texture_paths)
        : texture_unit(texture_unit) {
        texture = utils::Loader::getInstance().loadCubemap(texture_paths);
    }
    virtual ~Texture() { glDeleteTextures(1, &texture); }
    GLuint getTexture() const { return texture; }
    GLuint getTextureUnit() const { return texture_unit; }
    const char* getTextureUnitShaderName() const {
        return texture_unit_shader_name_map.at(texture_unit);
    }
    int getTextureUnitShaderID() const { return texture_unit_shader_id_map.at(texture_unit); }

private:
    GLuint texture;
    GLuint texture_unit;

    const static std::map<GLuint, const char*> texture_unit_shader_name_map;
    const static std::map<GLuint, int> texture_unit_shader_id_map;
};

using TexturePtr = std::unique_ptr<Texture>;
using TextureSPtr = std::shared_ptr<Texture>;