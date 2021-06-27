//
// Created by DamianPC on 9/20/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_ENTITY_ANIMATION_ANIMATION_CLIP_H_
#define ZOMBIESHOOTER_INCLUDE_ENTITY_ANIMATION_ANIMATION_CLIP_H_

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace ten_thousand_chambers {

class AnimationClip {
 public:
  AnimationClip(std::vector<TexturePtr>& sprites, sf::Time clip_duration, bool repeat = true) : sprites(sprites) {
    current_frame = 0;
    this->clip_duration = clip_duration;
    this->repeat = repeat;
  }

  void Update() {
    if (!repeat && current_frame == sprites.size() - 1)
      return;
    if (timer.getElapsedTime() >= clip_duration) {
      current_frame = (current_frame + 1) % sprites.size();
      timer.restart();
    }
  }

  void Reset() {
    current_frame = 0;
    timer.restart();
  }

  sf::Texture* GetCurrentFrame() { return sprites[current_frame].get(); }

 private:
  std::vector<TexturePtr>& sprites;
  unsigned current_frame;
  sf::Clock timer;
  sf::Time clip_duration;
  bool repeat;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_ENTITY_ANIMATION_ANIMATION_CLIP_H_
