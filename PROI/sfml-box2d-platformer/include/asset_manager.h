#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <Tmx.h>
#include <memory>

namespace ten_thousand_chambers {

enum class AnimationName{
  kPlayer,
};

enum class TextureName {
  kPlatform,
  kBackground,
  kWall,
  kDefaultTile
};

enum class TilemapName {
  kDefault
};

enum class ClipName {
  kIdle,
  kRun,
  kJump,
  kFall,
  kAny
};

struct Tilemap {
 public:
  Tmx::Map map;
  std::vector<sf::Texture> tileset_textures;
};

using TexturePtr = std::unique_ptr<sf::Texture>;
using Animation = std::map<ClipName, std::vector<TexturePtr>>;
using TilemapPtr = std::unique_ptr<Tilemap>;


class AssetManager {
 public:
  static AssetManager& GetInstance(){
    static AssetManager instance;
    return instance;
  }

  AssetManager(AssetManager const&) = delete;
  void operator=(AssetManager const&) = delete;

  void LoadStaticTexture(TextureName texture_name);
  void LoadAnimationSprites(AnimationName animation_name, const std::vector<ClipName>& defined_clips);
  void LoadTilemap(TilemapName tilemap_name);

  void FreeStaticTexture(TextureName texture_name);
  void FreeAnimationClip(AnimationName animation_name);
  void FreeTilemap(TilemapName tilemap_name);

  sf::Texture* GetStaticTexture(TextureName texture_name);
  std::vector<TexturePtr>& GetAnimationClipSprites(AnimationName asset_name, ClipName clip_name);
  Tilemap* GetTilemap(TilemapName tilemap_name);

 private:
  AssetManager(){}

  inline static AssetManager* instance = nullptr;

  const std::string sprites_dir = "resources/sprites/";
  const std::string static_dir = sprites_dir + "static/";
  const std::string animated_dir = sprites_dir + "animated/";
  const std::string tileset_dir = "resources/tilesets/";
  const std::string maps_dir = "resources/maps/";

  const std::map<TilemapName, std::string> tilemap_paths = {
      {TilemapName::kDefault, maps_dir + "default.tmx"},
  };

  const std::map<AnimationName, std::string> animation_paths = {
      {AnimationName::kPlayer, animated_dir + "player/" },
  };

  const std::map<TextureName, std::string> texture_paths = {
      {TextureName::kPlatform, static_dir + "platform.png" },
      {TextureName::kBackground, static_dir + "background.png" },
      {TextureName::kWall, static_dir + "wall.png"},
      {TextureName::kDefaultTile, tileset_dir + "default/tiles/tile000.png" },
  };

  const std::map<ClipName, std::string> clip_dir_name = {
      {ClipName::kIdle, "idle/" },
      { ClipName::kRun, "run/" },
      { ClipName::kJump, "jump/" },
      { ClipName::kFall, "fall/" }

  };

  std::map<AnimationName, Animation> animation_clips;
  std::map<TextureName, TexturePtr> static_textures;
  std::map<TilemapName, TilemapPtr> tilemaps;
};

}
#endif