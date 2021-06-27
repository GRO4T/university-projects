//
// Created by DamianPC on 9/20/2020.
//

#include <game.h>
#include "background_manager.h"

namespace ten_thousand_chambers {

BackgroundManager::BackgroundManager(sf::RenderWindow *window) {
  this->window = window;
}

void BackgroundManager::Draw() {
  /*
   * Algorithm:
   *  1. Calculate center background tile position
   *  2. Until view isn't fully filled draw background tiles at the edges
   */

  // Calculate
  bg_sprite.setTexture(*AssetManager::GetInstance().GetStaticTexture(current_background));
  auto view = window->getView();
  sf::Vector2f view_size = view.getSize();
  sf::Vector2f bg_size = sf::Vector2f(bg_sprite.getTexture()->getSize());
  // get view's left top corner position
  sf::Vector2f view_pos = { view.getCenter().x - view_size.x / 2, view.getCenter().y - view_size.y / 2 };
  // get a and b so a * view.size.x <= view_pos.x and b * view.size.y <= view_pos.y
  int a = int(view_pos.x) / int(view_size.x);
  int b = int(view_pos.y) / int(view_size.y);
  sf::Vector2f main_tile_pos = { a * view_size.x, b * view_size.y };

  // Draw
  std::vector<std::pair<int, int>> rect = {
    {-1, 3}, {0, 3}, {1, 3},
    {-1, 2}, {0, 2}, {1, 2},
    {-1, 1}, {0, 1}, {1, 1},
    {-1, 0}, {0, 0}, {1, 0},
    {-1, -1}, {0, -1}, {1, -1},
  };

  for (auto v : rect) {
    bg_sprite.setPosition(main_tile_pos.x + v.first * bg_size.x, main_tile_pos.y + v.second * bg_size.y);
    window->draw(bg_sprite);
  }
}

void ten_thousand_chambers::BackgroundManager::SetCurrentBackground(TextureName current_background) {
  BackgroundManager::current_background = current_background;
}

}
