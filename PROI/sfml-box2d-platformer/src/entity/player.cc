//
// Created by DamianPC on 9/15/2020.
//

#include <entity/player.h>
#include <debug_manager.h>

namespace ten_thousand_chambers {

Player::Player(const Box2DBodyData &body_data, const sf::Texture &texture) :
    Entity(body_data, texture),
    movement_controller(*this),
    animation_controller(this->sprite) {
  movement_controller.SetMaxSpeed(20.0f);
  movement_controller.SetJumpForce(120.0f);
  movement_controller.SetJumpHeight(3.0f);

}

void Player::Update() {
  movement_controller.Update();

  animation_controller.SetOnGround(movement_controller.IsOnGround());
  animation_controller.SetMovementSpeed(GetVelocity());
  if (std::abs(GetVelocityX()) > PlayerAnimationController::idle_speed_threshold)
    animation_controller.SetDirection(GetVelocityX() > 0.0f ? Direction::kRight : Direction::kLeft);
  animation_controller.Update();

  UpdateOnGround();
}


void Player::UpdateOnGround() {
  auto contact_list = body->GetContactList();

  do {
    if (contact_list == nullptr) {
      break;
    }
    Entity* e = static_cast<Entity*>(contact_list->other->GetUserData());
    if (e->IsOfType(EntityType::kPlatform) && GetWorldY() > e->GetWorldY()) {
      movement_controller.SetOnGround(true);
      movement_controller.SetLastGroundY(GetWorldY());
      return;
    }
    contact_list = contact_list->next;
  } while (contact_list != nullptr);
  movement_controller.SetOnGround(false);
}

void Player::OnCollisionStart(b2Contact *contact) {
}

void Player::OnCollisionEnd(b2Contact *contact) {
}

}




