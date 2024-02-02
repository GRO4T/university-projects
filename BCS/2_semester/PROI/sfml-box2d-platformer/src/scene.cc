//
// Created by DamianPC on 9/19/2020.
//

#include <scene.h>
#include <entity/entity_factory.h>
#include <game.h>
#include <Tmx.h>

namespace ten_thousand_chambers {

Scene::Scene(Camera& camera) :
    window(camera.GetWindow()),
    camera(camera) {}

void Scene::Load() {
  LoadAssets();
  Box2DWorld::Create();
  player = EntityFactory::CreateEntity(EntityType::kPlayer, sf::Vector2i(100, 50));
  CreateMap();
  camera.SetTarget(player.get());
}

void Scene::Destroy() {
  Box2DWorld::Destroy();
  player.reset();
  for (unsigned i = 0; i < entities.size(); i++) {
    entities[i].reset();
  }
  FreeAssets();
}

void Scene::Update() {
  camera.Update();
  player->Update();
  for (unsigned i = 0; i < entities.size(); i++) {
    entities[i]->Update();
  }
  Box2DWorld::Update();
}

void Scene::Draw() {
  for (unsigned i = 0; i < entities.size(); i++) {
    entities[i]->Draw(*window);
  }
  player->Draw(*window);
}

void Scene::LoadAssets() {
  asset_manager.LoadStaticTexture(TextureName::kBackground);
  asset_manager.LoadStaticTexture(TextureName::kPlatform);
  asset_manager.LoadStaticTexture(TextureName::kWall);
  asset_manager.LoadStaticTexture(TextureName::kDefaultTile);
  asset_manager.LoadAnimationSprites(AnimationName::kPlayer, PlayerAnimationController::GetDefinedClips());
  asset_manager.LoadTilemap(TilemapName::kDefault);
}

void Scene::FreeAssets() {
  asset_manager.FreeStaticTexture(TextureName::kBackground);
  asset_manager.FreeStaticTexture(TextureName::kPlatform);
  asset_manager.FreeStaticTexture(TextureName::kWall);
  asset_manager.FreeStaticTexture(TextureName::kDefaultTile);
  asset_manager.FreeAnimationClip(AnimationName::kPlayer);
  asset_manager.FreeTilemap(TilemapName::kDefault);
}

void Scene::CreateMap() {
  Tilemap* tilemap = asset_manager.GetTilemap(TilemapName::kDefault);
  std::vector<Tmx::TileLayer*> tile_layers = tilemap->map.GetTileLayers();

  for (auto tile_layer : tile_layers) {
    for (int y = 0; y < tilemap->map.GetHeight(); ++y) {
      for (int x = 0; x < tilemap->map.GetWidth(); ++x) {
        Tmx::MapTile tile = tile_layer->GetTile(x, y);
        // if gid is 0 then there is no tile
        if (tile.gid == 0)
          continue;
        auto tileset = tilemap->map.GetTileset(tile.tilesetId);
        sf::Texture& texture = tilemap->tileset_textures.at(tile.tilesetId);
        CreateTile(x, y, tile.gid, tileset, texture, tile_layer->GetName());
      }
    }
  }

}

void Scene::CreateTile(int x, int y, int gid, const Tmx::Tileset *tileset, sf::Texture &texture, std::string layer_name) {
  int width = tileset->GetTileWidth();
  int height = tileset->GetTileHeight();
  int tileset_cols = tileset->GetColumns();
  int tileset_rows = tileset->GetTileCount() / tileset_cols;

  Box2DBodyData body_data;
  body_data.body_type = b2_staticBody;
  body_data.size_in_pixels = sf::Vector2i(width, height);
  body_data.screen_position = sf::Vector2i(x * tileset->GetTileWidth(), y * tileset->GetTileHeight());

  // depending on layer name we create a tile of different type
  if (layer_name == "Ground")
    body_data.user_data = EntityType::kPlatform;
  else if (layer_name == "Walls")
    body_data.user_data = EntityType::kWall;
  else
    throw std::runtime_error("Unrecognized tile layer: " + layer_name);

  body_data.collisionCategory = CollisionMask::kStatic;
  body_data.collisionMask = CollisionMask::kDynamic;

  std::unique_ptr<Entity> entity = std::make_unique<Entity>(body_data, texture);
  entity->SetTextureRect(sf::IntRect(
      (gid - 1) % tileset_cols * width,
      (gid - 1) % tileset_rows * height,
      width,
      height
  ));
  entities.push_back(std::move(entity));
}

}
