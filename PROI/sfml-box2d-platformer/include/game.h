#ifndef GAME_H
#define GAME_H

#include <entity/entity_factory.h>
#include <camera.h>
#include <scene.h>
#include "background_manager.h"

namespace ten_thousand_chambers {

class Game {
 public:

  Game(sf::RenderWindow *window);
  void Run(bool debug_mode = false);

  constexpr static const float fps_target = 60.0f;
  inline static sf::Time delta_time;

  static bool IsClosed();

  Camera &GetCamera();

  static sf::RenderWindow *GetWindow();

 private:
  void HandleEvents();

  virtual void GameLoop();

  void InitState();

  void ResetState();

  inline static sf::RenderWindow *window;
  Camera camera;
  AssetManager &asset_manager;

  BackgroundManager background_manager;
  Scene scene;

  std::unique_ptr<Entity> player;
  std::vector<std::unique_ptr<Entity>> entities;

  inline static bool closed = false;
  inline static bool debug_mode = false;
};

}

#endif
