//
// Created by DamianPC on 9/19/2020.
//

#ifndef ZOMBIESHOOTER_INCLUDE_SCENE_H_
#define ZOMBIESHOOTER_INCLUDE_SCENE_H_

#include <iostream>
#include "asset_manager.h"
#include "camera.h"

namespace ten_thousand_chambers {

class Scene {
 public:
  Scene(Camera& camera);

  void Load();
  void Destroy();
  void Update();
  void Draw();

 private:
  void LoadAssets();
  void FreeAssets();
  void CreateMap();
  void CreateTile(int x, int y, int gid, const Tmx::Tileset *tileset, sf::Texture &texture, std::string layer_name);

  AssetManager& asset_manager = AssetManager::GetInstance();
  Camera& camera;
  sf::RenderWindow* window;
  std::unique_ptr<Entity> player;

  // EventListener event_listener;
  // GUIManager gui_manager;

  // std::map<BackgroundType, sf::Texture> backgrounds;
  std::vector<std::unique_ptr<Entity>> entities;
  // std::vector<GUIElement> gui;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_SCENE_H_
