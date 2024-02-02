//
// Created by damian on 8/19/20.
//

#ifndef ZOMBIESHOOTER_INCLUDE_GUI_CAMERA_H_
#define ZOMBIESHOOTER_INCLUDE_GUI_CAMERA_H_

#include <entity/entity.h>

namespace ten_thousand_chambers {

class Camera {
 public:
  Camera(sf::RenderWindow *window);
  void SetTarget(Entity *target);
  sf::Vector2f GetCameraView();
  void Update();
  sf::View *GetView();
  void SetView(unsigned int width, unsigned int height, sf::Vector2f center);
  sf::RenderWindow *GetWindow() const;

 private:
  float last_target_x;
  float last_target_y;
  float follow_offset_x = 10;
  float follow_offset_y = 10;
  float zoom = 1.0f;

  //sf::Vector2f look_at_offset = sf::Vector2f(0, -30);
  float speed = 5.0f;

  sf::RenderWindow *window;
  unsigned int screen_width;
  unsigned int screen_height;

  sf::View view;
  Entity *target;

};

}

#endif //ZOMBIESHOOTER_INCLUDE_GUI_CAMERA_H_
