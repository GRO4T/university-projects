//
// Created by damian on 8/19/20.
//

#include <game.h>

namespace ten_thousand_chambers {

Camera::Camera(sf::RenderWindow *window) {
  this->window = window;
  screen_width = window->getSize().x;
  screen_height = window->getSize().y;
  view = sf::View(
      sf::Vector2f(0.0f, 0.0f),
      sf::Vector2f(zoom * screen_width, zoom * screen_height)
      );
}

void Camera::SetTarget(Entity *target) {
  this->target = target;
  float targetCenterX = target->GetScreenX() + target->GetSizeInPixels().x / 2;
  float targetCenterY = target->GetScreenY() + target->GetSizeInPixels().y / 2;
  view.setCenter(targetCenterX, targetCenterY);
  last_target_y = targetCenterX;
  last_target_x = targetCenterY;
}

sf::Vector2f Camera::GetCameraView() { return sf::Vector2f(last_target_x, last_target_y); }

void Camera::Update() {
  if (target != nullptr) {
    // follow target
    float target_center_x = target->GetScreenX() + target->GetSizeInPixels().x / 2;
    float target_center_y = target->GetScreenY() + target->GetSizeInPixels().y / 2;

    float effective_speed = speed * Game::delta_time.asSeconds();
    float new_x = std::abs(last_target_x - target_center_x) > follow_offset_x ?
                  last_target_x + (target_center_x - last_target_x) * effective_speed :
                  last_target_x;
    float new_y = std::abs(last_target_y - target_center_y) > follow_offset_y ?
                  last_target_y + (target_center_y - last_target_y) * effective_speed :
                  last_target_y;

    view.setCenter(new_x, new_y);
    window->setView(view);

    last_target_x = new_x;
    last_target_y = new_y;
  }
}

sf::View *Camera::GetView() { return &view; }

void Camera::SetView(unsigned int width, unsigned int height, sf::Vector2f center) {
  view.setSize(width * zoom, height * zoom);
  view.setCenter(center.x, center.y);
  window->setView(view);
}
sf::RenderWindow *Camera::GetWindow() const {
  return window;
}

}

