
#include<iostream>
#include <asset_manager.h>
#include <utils.h>

namespace ten_thousand_chambers {

void AssetManager::LoadStaticTexture(TextureName texture_name) {
  std::string filename = texture_paths.at(texture_name);
  //sf::Texture texture;
  TexturePtr texture_ptr = std::make_unique<sf::Texture>();
  if (texture_ptr->loadFromFile(filename)){
    static_textures.emplace(texture_name, std::move(texture_ptr));
  }
  else{
    throw std::runtime_error("Cannot load texture:" + filename);
  }
}

void AssetManager::LoadAnimationSprites(AnimationName animation_name, const std::vector<ClipName> &defined_clips) {
  std::string asset_root_dir = animation_paths.at(animation_name);
  Animation animation;
  for (ClipName clip_name : defined_clips){
    std::vector<std::string> file_list;
    std::string clip_dir = asset_root_dir + clip_dir_name.at(clip_name);
    Utils::ReadDirectory(clip_dir, file_list);
    std::vector<TexturePtr> sprites;
    for (auto filename : file_list) {
      TexturePtr texture_ptr = std::make_unique<sf::Texture>();
      if (texture_ptr->loadFromFile(clip_dir + filename)) {
        sprites.push_back(std::move(texture_ptr));
      }
      else {
        throw std::runtime_error("Cannot load texture:" + filename);
      }
    }
    animation.emplace(clip_name, std::move(sprites));
  }
  animation_clips.emplace(animation_name, std::move(animation));
}

sf::Texture* AssetManager::GetStaticTexture(TextureName texture_name){
  return static_textures.at(texture_name).get();
}

std::vector<TexturePtr>& AssetManager::GetAnimationClipSprites(AnimationName asset_name, ClipName clip_name) {
  return animation_clips.at(asset_name).at(clip_name);
}

void AssetManager::LoadTilemap(TilemapName tilemap_name) {
  TilemapPtr tilemap_ptr = std::make_unique<Tilemap>();
  tilemap_ptr->map.ParseFile(tilemap_paths.at(TilemapName::kDefault));
  // load tilesets
  std::vector<Tmx::Tileset*> tilesets = tilemap_ptr->map.GetTilesets();
  for (auto tileset : tilesets) {
    std::string name = tileset->GetName();
    std::string path = tileset->GetImage()->GetSource();
    sf::Texture texture;
    texture.loadFromFile("resources/tilesets/" + name + "/" + path);
    tilemap_ptr->tileset_textures.push_back(texture);
  }
  // I had to use this form, because Tmx::Map doesn't have copy and move constructors
  tilemaps[tilemap_name] = static_cast<std::unique_ptr<Tilemap> &&>(tilemap_ptr);
}

Tilemap* AssetManager::GetTilemap(TilemapName tilemap_name) {
  return tilemaps.at(tilemap_name).get();
}

void AssetManager::FreeStaticTexture(TextureName texture_name) {
  static_textures.erase(texture_name);
}

void AssetManager::FreeAnimationClip(AnimationName animation_name) {
  animation_clips.erase(animation_name);
}

void AssetManager::FreeTilemap(TilemapName tilemap_name) {
  tilemaps.erase(tilemap_name);
}

}

