//
// Created by DamianPC on 9/20/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_DEBUG_MANAGER_H_
#define ZOMBIESHOOTER_INCLUDE_DEBUG_MANAGER_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include <SFML/Graphics/Text.hpp>

namespace ten_thousand_chambers {

class DebugManager {
 public:
  static DebugManager& GetInstance();

  DebugManager(DebugManager const&) = delete;
  void operator=(DebugManager const&) = delete;

  void Debug(std::string name, double value);
  void Debug(std::string name, bool value);
  void DisplayVariableWatch();
  void Clear();

 private:
  DebugManager();

  sf::RenderWindow* window;
  sf::Font font;
  const int font_size = 11;
  sf::Vector2f msg_offset = {10, 10 };
  std::map<std::string, double> doubles;
  std::map<std::string, bool> bools;
};

}


#endif //ZOMBIESHOOTER_INCLUDE_DEBUG_MANAGER_H_
