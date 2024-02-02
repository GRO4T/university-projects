//
// Created by DamianPC on 9/15/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_ANIMATION_H_
#define ZOMBIESHOOTER_INCLUDE_ANIMATION_H_

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <asset_manager.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <initializer_list>
#include "animation_clip.h"

namespace ten_thousand_chambers {

using TransitionCond = std::function<bool()>;
using Transitions = std::vector<std::pair<ClipName, TransitionCond>>;

enum class Direction {
  kLeft, kRight
};

class AnimationController : public Behaviour {
 public:
  void Update() override {
    current_clip = animation_graph.Transition(current_clip);
    AnimationClip& clip = clips.at(current_clip);
    clip.Update();
    entity_sprite.setTexture(*clip.GetCurrentFrame());
  }

  void SetDirection(Direction direction) {
    AnimationController::direction = direction;
  }

 protected:
  class AnimationGraph {
   public:
    void AddEdge(ClipName src, ClipName dest, TransitionCond transition_cond) {
      if (transition_list.find(src) == transition_list.end()) {
        transition_list.insert(std::make_pair(src, Transitions() ));
      }
      transition_list.at(src).push_back(std::make_pair(dest, transition_cond));
    }

    ClipName Transition(ClipName current_clip) {
      if (transition_list.find(ClipName::kAny) != transition_list.end()) {
        auto transitions = transition_list.at(ClipName::kAny);
        for (auto transition : transitions) {
          if (transition.second())
            return transition.first;
        }
      }
      if (transition_list.find(current_clip) != transition_list.end()) {
        auto transitions = transition_list.at(current_clip);
        for (auto transition : transitions) {
          if (transition.second())
            return transition.first;
        }
      }
      return current_clip;
    }

   private:
    std::map<ClipName, Transitions> transition_list;
  };

  AnimationController(
      sf::Sprite& entity_sprite,
      std::initializer_list<std::pair<const ClipName, AnimationClip>> clips
      ):
      entity_sprite(entity_sprite), clips(clips) {}

  std::map<ClipName, AnimationClip> clips;
  ClipName current_clip = ClipName::kIdle;
  sf::Sprite& entity_sprite;
  AnimationGraph animation_graph;
  Direction direction;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_ANIMATION_H_
