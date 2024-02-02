#include <entity/entity.h>

namespace ten_thousand_chambers {

Entity::Entity(const Box2DBodyData &body_data, const sf::Texture& texture) {
  size_in_pixels = body_data.size_in_pixels;
  size_in_meters = b2Vec2(
      size_in_pixels.x / (2.0f * Box2DWorld::pixels_per_meter),
      size_in_pixels.y / (2.0f * Box2DWorld::pixels_per_meter)
      );
  entity_type = body_data.user_data;

  SetBody(Box2DWorld::GetWorld(), body_data.screen_position, body_data.body_type);
  SetFixture(body_data.size_in_pixels, body_data.collisionCategory, body_data.collisionMask);
  body->SetUserData(this);

  sprite.setTexture(texture);
  sprite.setTextureRect(sf::IntRect(0, 0, size_in_pixels.x, size_in_pixels.y));
  sprite.setPosition(GetWorldX(), GetWorldY());
}

void Entity::Move(b2Vec2 force) { body->ApplyLinearImpulse(force, body->GetWorldCenter(), true); }

void Entity::Move(float vx, float vy) { body->SetLinearVelocity(b2Vec2(vx, vy)); }

void Entity::Move(b2Vec2 impulse, float maxSpeed) {
  body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
  body->SetLinearVelocity(
      b2Vec2(
          CLAMP(body->GetLinearVelocity().x,-maxSpeed, maxSpeed),
          body->GetLinearVelocity().y
          )
      );
}

void Entity::Draw(sf::RenderWindow &window) {
  sprite.setPosition(GetScreenX(), GetScreenY());

  auto view = window.getView();
  auto view_bounds = sf::Rect<float>(
      view.getCenter().x - view.getSize().x / 2,
      view.getCenter().y - view.getSize().y / 2,
      view.getSize().x,
      view.getSize().y
      );
  auto texture_bounds = sprite.getGlobalBounds();

  // check if sprite is in viewport
  if (texture_bounds.left + texture_bounds.width > view_bounds.left &&
      texture_bounds.top + texture_bounds.height > view_bounds.top &&
      texture_bounds.left < view_bounds.left + view_bounds.width &&
      texture_bounds.top < view_bounds.top + view_bounds.height) {
    window.draw(sprite);
  }
}

void Entity::SetTextureRect(const sf::IntRect &texture_rect) {
  sprite.setTextureRect(texture_rect);
  auto size = sprite.getTextureRect().width;
}

float Entity::GetScreenX() const {
  if (sprite.getScale().x > 0.0f)
    return body->GetPosition().x * Box2DWorld::pixels_per_meter - size_in_pixels.x / 2;
  else
    return body->GetPosition().x * Box2DWorld::pixels_per_meter + size_in_pixels.x / 2;
}

float Entity::GetScreenY() const {
  return -body->GetPosition().y * Box2DWorld::pixels_per_meter - size_in_pixels.y / 2;
}

void Entity::SetBody(b2World *world, sf::Vector2i screen_position, uint type) {
  b2BodyDef bodyDef;
  bodyDef.position.Set(
      (screen_position.x + size_in_pixels.x / 2) / Box2DWorld::pixels_per_meter,
      -(screen_position.y + size_in_pixels.y / 2) / Box2DWorld::pixels_per_meter
      );
  bodyDef.type = static_cast<b2BodyType>(type);
  body = world->CreateBody(&bodyDef);
}

void Entity::SetFixture(sf::Vector2i size_in_pixels, uint16 category_bits, uint16 mask_bits) {
  b2PolygonShape shape;
  shape.SetAsBox(size_in_meters.x, size_in_meters.y);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = 1.0f;
  myFixtureDef.friction = 0.0f;
  myFixtureDef.filter.categoryBits = category_bits;
  myFixtureDef.filter.maskBits = mask_bits;
  body->CreateFixture(&myFixtureDef);
}

bool Entity::IsOfType(EntityType::EntityType entity_type) {
  return this->entity_type & entity_type;
}

}
