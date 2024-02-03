#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <functional>

#include "../utils.h"
#include "../world.h"

namespace ten_thousand_chambers {

struct Box2DBodyData;

namespace EntityType{
enum EntityType {
  kStatic = 0x0001,
  kDynamic = 0x0002,
  kPlatform = 0x0004,
  kPlayer = 0x0008,
  kWall = 0x0010,
};
}

namespace CollisionMask{
enum CollisionMask {
  kStatic = 0x0001,
  kDynamic = 0x0002,
  kAll = 0xFFFF
};
}


class Entity {
 public:
  Entity() {}
  Entity(const Box2DBodyData& body_data, const sf::Texture& texture);

  virtual void Update() {}

  void Move(b2Vec2 force);
  void Move(float vx, float vy);
  void Move(b2Vec2 impulse, float maxSpeed);

  virtual void OnCollisionStart(b2Contact* contact) {}
  virtual void OnCollisionEnd(b2Contact* contact) {}

  void Draw(sf::RenderWindow &window);
  void SetTextureRect(const sf::IntRect& texture_rect);

  float GetWorldX() const { return body->GetPosition().x; }
  float GetWorldY() const { return body->GetPosition().y; }
  b2Vec2 GetWorldPosition() const { return body->GetPosition(); }
  float GetScreenX() const;
  float GetScreenY() const;
  sf::Vector2f GetScreenPosition() const { return sf::Vector2f(GetScreenX(), GetScreenY()); }
  sf::Vector2i GetSizeInPixels() const { return size_in_pixels; }
  b2Vec2 GetSizeInMeters() const { return size_in_meters; }
  uint GetEntityType() { return entity_type; }
  float GetVelocityX() { return body->GetLinearVelocity().x; }
  float GetVelocityY() { return body->GetLinearVelocity().y; }
  const b2Vec2& GetVelocity() { return body->GetLinearVelocity(); }

  bool IsOfType(EntityType::EntityType entity_type);

 protected:
  b2Body *body;
  sf::Vector2i size_in_pixels;
  b2Vec2 size_in_meters;
  uint entity_type;

  sf::Sprite sprite;

  void SetBody(b2World *world, sf::Vector2i position, uint type);
  virtual void SetFixture(sf::Vector2i size, uint16 categoryBits, uint16 maskBits);
};

struct Box2DBodyData {
  uint body_type;
  sf::Vector2i size_in_pixels;
  sf::Vector2i screen_position;
  uint user_data;
  uint16 collisionCategory;
  uint16 collisionMask;
};

}

#endif