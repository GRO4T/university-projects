//
// Created by damian on 8/17/20.
//

#ifndef ZOMBIESHOOTER_INCLUDE_ENTITY_FACTORY_H_
#define ZOMBIESHOOTER_INCLUDE_ENTITY_FACTORY_H_

#include <iostream>
#include <stdexcept>

#include "entity/player.h"
#include "asset_manager.h"

namespace ten_thousand_chambers {

class EntityFactory {
 public:
  EntityFactory(): asset_manager(AssetManager::GetInstance()) {}

  static std::unique_ptr<Entity> CreateEntity(EntityType::EntityType entity_type, sf::Vector2i screen_position){
    switch(entity_type){
      case EntityType::kPlatform: return CreatePlatform(screen_position);
      case EntityType::kPlayer: return CreatePlayer(screen_position);
      case EntityType::kWall: return CreateWall(screen_position);
      default: throw std::runtime_error("Exception::EntityFactory::UnhandledEntityType");
    }
  }

 private:
  static std::unique_ptr<Entity> CreatePlatform(sf::Vector2i screen_position){
    Box2DBodyData body_data;
    body_data.body_type = b2_staticBody;
    // sf::Texture& texture = AssetManager::GetInstance().GetStaticTexture(AssetName::kTexturePlatform);
    sf::Texture& texture = *(AssetManager::GetInstance().GetStaticTexture(TextureName::kDefaultTile));
    body_data.size_in_pixels = sf::Vector2i(texture.getSize().x, texture.getSize().y);
    body_data.screen_position = screen_position;
    body_data.user_data = EntityType::kPlatform;
    body_data.collisionCategory = CollisionMask::kStatic;
    body_data.collisionMask = CollisionMask::kDynamic;

    return std::make_unique<Entity>(body_data, texture);
  }

  static std::unique_ptr<Entity> CreateWall(sf::Vector2i screen_position) {
    Box2DBodyData body_data;
    body_data.body_type = b2_staticBody;
    sf::Texture* texture = AssetManager::GetInstance().GetStaticTexture(TextureName::kWall);
    body_data.size_in_pixels = sf::Vector2i(texture->getSize().x, texture->getSize().y);
    body_data.screen_position = screen_position;
    body_data.user_data = EntityType::kWall;
    body_data.collisionCategory = CollisionMask::kStatic;
    body_data.collisionMask = CollisionMask::kDynamic;

    return std::make_unique<Entity>(body_data, *texture);
  }

  static std::unique_ptr<Entity> CreatePlayer(sf::Vector2i screen_position){
    Box2DBodyData body_data;
    body_data.body_type = b2_dynamicBody;
    sf::Texture* texture =
        AssetManager::GetInstance().GetAnimationClipSprites(
            AnimationName::kPlayer,
            ClipName::kIdle
            )[0].get();
    body_data.size_in_pixels = sf::Vector2i(texture->getSize().x, texture->getSize().y);
    body_data.screen_position = screen_position;
    body_data.user_data = EntityType::kPlayer;
    body_data.collisionCategory = CollisionMask::kDynamic;
    body_data.collisionMask = CollisionMask::kStatic;

    return std::make_unique<Player>(body_data, *texture);
  }

 private:
  AssetManager& asset_manager;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_ENTITY_FACTORY_H_
