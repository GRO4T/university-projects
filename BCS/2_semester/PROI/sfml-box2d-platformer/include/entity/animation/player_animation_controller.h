//
// Created by DamianPC on 9/15/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_ENTITY_ANIMATION_PLAYER_ANIMATION_CONTROLLER_H_
#define ZOMBIESHOOTER_INCLUDE_ENTITY_ANIMATION_PLAYER_ANIMATION_CONTROLLER_H_

#include <entity/animation/animation_controller.h>
#include <SFML/Graphics/Sprite.hpp>

namespace ten_thousand_chambers {

class PlayerAnimationController : public AnimationController {
 public:
  PlayerAnimationController(sf::Sprite &entity_sprite) :
      AnimationController(
          entity_sprite,
          {
              { DefineClip(ClipName::kIdle, 0.25f) },
              { DefineClip(ClipName::kRun, 0.1f) },
              { DefineClip(ClipName::kJump, 0.25f, false) },
              { DefineClip(ClipName::kFall, 0.25f) }
          }
      ) {
    BuildAnimationGraph();
  }

  void BuildAnimationGraph() {
    animation_graph.AddEdge(ClipName::kAny, ClipName::kJump, [this]() { return !on_ground && movement_speed.y >= 0; });
    animation_graph.AddEdge(ClipName::kAny, ClipName::kFall, [this]() { return !on_ground && movement_speed.y < 0; });
    animation_graph.AddEdge(ClipName::kAny, ClipName::kIdle, [this]() { return std::abs(movement_speed.x) <= idle_speed_threshold; });
    animation_graph.AddEdge(ClipName::kAny, ClipName::kRun, [this]() { return std::abs(movement_speed.x) > idle_speed_threshold; });
  }

  void Update() {
    AnimationController::Update();
    entity_sprite.setScale(direction == Direction::kRight ? 1.0f : -1.0f, 1.0f);
  }

  static std::vector<ClipName> GetDefinedClips() {
    return { ClipName::kIdle, ClipName::kRun, ClipName::kJump, ClipName::kFall };
  }

  void SetOnGround(bool on_ground) {
    PlayerAnimationController::on_ground = on_ground;
  }

  void SetMovementSpeed(const b2Vec2 &movement_speed) {
    PlayerAnimationController::movement_speed = movement_speed;
  }

  static constexpr float idle_speed_threshold = 0.05f;

 private:
  std::pair<ClipName, AnimationClip> DefineClip(ClipName clip_name, float clip_duration, bool repeat = true) {
    return std::make_pair(
        clip_name,
        AnimationClip(
            AssetManager::GetInstance().GetAnimationClipSprites(AnimationName::kPlayer, clip_name),
            sf::seconds(clip_duration),
            repeat)
    );
  }

  bool on_ground = false;
  b2Vec2 movement_speed;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_ENTITY_ANIMATION_PLAYER_ANIMATION_CONTROLLER_H_
