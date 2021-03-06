// Copyright 2015, Aaron Ceross

#include <string>
#include <iostream>
#include "../include/entity.hpp"

// The base class for game entities
// Able to be extended into actual game play with damage and removal functions.
// Not all features are currently implemented.
Entity::Entity()
: movement_()
, hit_points_()
{}

int Entity::GetHitPoints() const {
  return hit_points_;
}

void Entity::Damage(int points) {
  assert(points > 0);
  hit_points_ -= points;
}

void Entity::Destroy() {
  hit_points_ = 0;
}

void Entity::Remove() {
  Destroy();
}

bool Entity::IsDestroyed() const {
  return hit_points_ <= 0;
}

void Entity::UpdateCurrent(sf::Time delta_time, CommandQueue &commands) {
  move(movement_ * delta_time.asSeconds());
}
