//
// Created by DamianPC on 8/29/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_ENTITY_PLAYER_H_
#define ZOMBIESHOOTER_INCLUDE_ENTITY_PLAYER_H_

#include <entity/movement_controller.h>
#include <entity/animation/player_animation_controller.h>
#include <iostream>

namespace ten_thousand_chambers {

class Player : public Entity{
 public:
  Player(const Box2DBodyData& body_data, const sf::Texture& texture);

  void Update() override;

  void OnCollisionStart(b2Contact *contact) override;
  void OnCollisionEnd(b2Contact *contact) override;

 private:
  void UpdateOnGround();

  MovementController movement_controller;
  PlayerAnimationController animation_controller;
};

}
#endif //ZOMBIESHOOTER_INCLUDE_ENTITY_PLAYER_H_
