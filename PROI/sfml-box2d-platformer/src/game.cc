#include <game.h>
#include <debug_manager.h>

namespace ten_thousand_chambers {

Game::Game(sf::RenderWindow *window):
    asset_manager(AssetManager::GetInstance()),
    camera(window),
    scene(camera),
    background_manager(window) {
  srand((unsigned int)time(NULL));
  delta_time = sf::seconds(1.0f / fps_target);
  this->window = window;
}

void Game::Run(bool debug_mode) {
  this->debug_mode = debug_mode;

    do {
      InitState();
      GameLoop();
      ResetState();
    } while (!closed);
}

void Game::GameLoop() {
  //debug
  sf::Clock timer;
  double fps = Game::fps_target;

  while (window->isOpen()) {
    for (int32 i = 0; i < Game::fps_target; ++i) {
      scene.Update();

      window->clear(sf::Color::Black);
      background_manager.Draw();
      scene.Draw();
      if (debug_mode)
        DebugManager::GetInstance().DisplayVariableWatch();
      window->display();

      HandleEvents();

      if (Game::IsClosed())
        return;
    }
    fps = Game::fps_target / timer.restart().asSeconds();
    DebugManager::GetInstance().Debug("fps", fps);
  }
}

void Game::HandleEvents() {
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:window->close();
        closed = true; break;
      default:break;
    }
  }
}

void Game::InitState() {
  scene.Load();
}

void Game::ResetState() {
  scene.Destroy();
}

bool Game::IsClosed() {
  return closed;
}

Camera &Game::GetCamera() {
  return camera;
}
sf::RenderWindow *Game::GetWindow() {
  return window;
}

}