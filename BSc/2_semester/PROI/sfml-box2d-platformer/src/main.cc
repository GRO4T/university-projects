#include <iostream>
#include <game.h>

int main() {
  sf::RenderWindow mainWindow(sf::VideoMode(1200, 600), "10 Thousand Chambers",
                              sf::Style::Close | sf::Style::Titlebar);
  mainWindow.setFramerateLimit((unsigned int)(ten_thousand_chambers::Game::fps_target));

  ten_thousand_chambers::Game game(&mainWindow);
  game.Run(true);

  return 0;
}