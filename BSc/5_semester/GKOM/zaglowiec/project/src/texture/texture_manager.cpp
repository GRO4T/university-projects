#include "texture/texture_manager.hpp"

std::map<TextureManager::Filepath, TextureSPtr> TextureManager::texture_map;
std::map<std::string, TextureSPtr> TextureManager::cubemap_texture_map;

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

TextureSPtr TextureManager::getTexture(const std::string& filepath) {
    auto it = texture_map.find(filepath);
    if (it != texture_map.end()) {
        return it->second;
    } else {
        TextureSPtr texture = std::make_shared<Texture>(GL_TEXTURE0, filepath);
        texture_map.insert(std::pair(filepath, texture));
        return texture;
    }
}

void TextureManager::loadTexture(const std::string& filepath) {
    TextureSPtr texture = std::make_shared<Texture>(GL_TEXTURE0, filepath);
    texture_map.insert(std::pair(filepath, texture));
}

TextureSPtr TextureManager::getLoadCubemapTexture(const std::string& cubemapName,
                                                  const std::vector<std::string>& filepaths) {
    try {
        return cubemap_texture_map.at(cubemapName);
    } catch (...) {  // std::out_of_range
        TextureSPtr texture = std::make_shared<Texture>(GL_TEXTURE0, filepaths);
        cubemap_texture_map.insert(std::pair(cubemapName, texture));
        return texture;
    }
}
