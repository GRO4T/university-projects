//
// Created by DamianPC on 9/20/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_BACKGROUND_MANAGER_H_
#define ZOMBIESHOOTER_INCLUDE_BACKGROUND_MANAGER_H_

#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "asset_manager.h"

namespace ten_thousand_chambers {

enum class BackgroundType {
  kDefault
};

class BackgroundManager {
 public:
  BackgroundManager(sf::RenderWindow* window);
  void Draw();
  void SetCurrentBackground(TextureName current_background);

 private:
  sf::RenderWindow* window;
  TextureName current_background = TextureName::kBackground;
  sf::Sprite bg_sprite;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_BACKGROUND_MANAGER_H_
