//
// Created by DamianPC on 9/13/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_BEHAVIOUR_BEHAVIOUR_H_
#define ZOMBIESHOOTER_INCLUDE_BEHAVIOUR_BEHAVIOUR_H_

#include <entity/entity.h>
#include <iostream>
#include <debug_manager.h>
#include "behaviour.h"

namespace ten_thousand_chambers {

class MovementController : public Behaviour {
 public:
  MovementController(Entity &entity) : entity(entity) {
    jump_timer.restart();
  }

  void Update() override {
    b2Vec2 movement(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      movement.x = 1000.0f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      movement.x = -1000.0f;
    }

    if (on_ground) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && jump_timer.getElapsedTime() >= jump_delay) {
        movement.y = jump_force;
        jump_timer.restart();
      }
    }

    entity.Move(0, entity.GetVelocityY());
    entity.Move(movement, max_speed);
    if (entity.GetWorldY() >= jump_height + last_ground_y || entity.GetVelocityY() < 0.0f) {
      entity.Move(b2Vec2(0, Box2DWorld::gravity));
    }

    DebugManager::GetInstance().Debug("on ground", on_ground);
  }

  void SetMaxSpeed(float max_speed) {
    MovementController::max_speed = max_speed;
  }
  void SetJumpForce(float jump_force) {
    MovementController::jump_force = jump_force;
  }
  void SetOnGround(bool on_ground) {
    MovementController::on_ground = on_ground;
  }
  void SetLastGroundY(float last_ground_y) {
    MovementController::last_ground_y = last_ground_y;
  }
  void SetJumpHeight(float jump_height) {
    MovementController::jump_height = jump_height;
  }

  bool IsOnGround() const {
    return on_ground;
  }

 public:
 private:
  Entity &entity;
  float max_speed;
  float jump_force;
  bool on_ground = false;
  float last_ground_y;                                  // y coordinate of last ground block player was standing on
  float jump_height;                                    // it isn't necessarily how high player will jump,
                                                        // but after reaching this height we will apply additional gravity
  sf::Clock jump_timer;
  sf::Time jump_delay = sf::seconds(0.25f);
};

}

#endif //ZOMBIESHOOTER_INCLUDE_BEHAVIOUR_BEHAVIOUR_H_
