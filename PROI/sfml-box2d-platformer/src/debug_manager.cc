//
// Created by DamianPC on 9/20/2020.
//

#include <debug_manager.h>
#include <game.h>

namespace ten_thousand_chambers {

DebugManager& DebugManager::GetInstance() {
  static DebugManager instance;
  return instance;
}

DebugManager::DebugManager(): window(Game::GetWindow()) {
  font.loadFromFile("resources/font/arial.ttf");
}

void DebugManager::Debug(std::string name, double value) {
  auto it = doubles.find(name);
  if (it != doubles.end()) {
    it->second = value;
  }
  else {
    doubles.insert(std::make_pair(name, value));
  }
}

void DebugManager::Debug(std::string name, bool value) {
  auto it = bools.find(name);
  if (it != bools.end()) {
    it->second = value;
  }
  else {
    bools.insert(std::make_pair(name, value));
  }
}

void DebugManager::Clear() {
  doubles.clear();
  bools.clear();
}

void DebugManager::DisplayVariableWatch() {
  std::string debug_msg = "";
  for (auto d : doubles) {
    debug_msg += d.first + ": " + std::to_string(d.second) + "\n";
  }
  for (auto b : bools) {
    std::string value = b.second == true ? "true" : "false";
    debug_msg += b.first + ": " + value + "\n";
  }

  auto view_pos = window->getView().getCenter();
  auto view_size = window->getView().getSize();
  sf::Text text(debug_msg, font, font_size);
  text.setPosition(view_pos.x - view_size.x / 2 + msg_offset.x, view_pos.y - view_size.y / 2 + msg_offset.y);
  window->draw(text);
}

}

