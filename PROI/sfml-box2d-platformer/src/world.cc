//
// Created by damian on 8/19/20.
//

#include <game.h>

namespace ten_thousand_chambers {

void ContactListener::BeginContact(b2Contact *contact) {
  void *bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
  Entity *e = static_cast<Entity *>(bodyUserData);
  e->OnCollisionStart(contact);
}

void ContactListener::EndContact(b2Contact *contact) {
  void *bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
  Entity *e = static_cast<Entity *>(bodyUserData);
  e->OnCollisionEnd(contact);
}

b2World *Box2DWorld::GetWorld() {
  return world;
}

void Box2DWorld::Create() {
  world = new b2World(b2Vec2(0.0f, gravity));
  world->SetAllowSleeping(true); //improves performance by not calculating inactive objects
  contact_listener.SetWorld(world);
  world->SetContactListener(&contact_listener);
  time_step = 1.0f / Game::fps_target;
}

void Box2DWorld::Update() {
  world->Step(time_step, velocity_iterations, position_iterations);
  world->ClearForces();
}

void Box2DWorld::Destroy() {
  delete world;
}

}

