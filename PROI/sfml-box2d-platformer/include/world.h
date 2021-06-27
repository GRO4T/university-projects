//
// Created by damian on 8/19/20.
//

#ifndef ZOMBIESHOOTER_INCLUDE_WORLD_H_
#define ZOMBIESHOOTER_INCLUDE_WORLD_H_

#ifdef _WIN32
  #include <box2d/box2d.h>
#elif __linux__
  #include <Box2D/Box2D.h>
#endif

namespace ten_thousand_chambers{

class ContactListener : public b2ContactListener {
 public:
  void SetWorld(b2World *w) { world = w; }

  inline static const float collision_epsilon = 0.25f;
 protected:
  void BeginContact(b2Contact *contact) override;
  void EndContact(b2Contact *contact) override;

  b2World *world;
};

class Box2DWorld {
 public:

  static b2World *GetWorld();

  static void Create();

  static void Update();

  static void Destroy();

  constexpr static const float pixels_per_meter = 16.0f;
  constexpr static const float gravity = -10.0f;
 private:

  inline static b2World* world;
  inline static ContactListener contact_listener;

  inline static float time_step;
  static const int velocity_iterations = 4;
  static const int position_iterations = 2;
};

}

#endif //ZOMBIESHOOTER_INCLUDE_WORLD_H_
