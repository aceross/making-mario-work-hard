// Copyright 2015, Aaron Ceross

#include <SFML/Graphics.hpp>
#include "../include/player.hpp"
#include "../include/game_state_manager.hpp"

Player::Player() :
sf::Sprite(texture), sprite_clock_delimiter(sf::seconds(0.5f)),
                                                            current_frame(0) {}

Player::Player(const sf::Texture& t) :
sf::Sprite(t), sprite_clock_delimiter(sf::seconds(0.5f)), current_frame(0) {}

Player::Player(const sf::Texture &t, const sf::IntRect &ir) :
sf::Sprite(t), sprite_clock_delimiter(sf::seconds(0.5f)), current_frame(0) {
  sprite_frames.push_back(ir);
}

void Player::update() {
  // Check to see if elapsed time is greater than delimiter time
  if (sprite_clock.getElapsedTime() > sprite_clock_delimiter) {
    // Check to see if we have to reset the value of current_frame
    if (sprite_frames.size() >= current_frame) {
      ++current_frame;
    } else {
      // Reset the value
      current_frame = 0;
    }
    // Set the clipping rect
    setTextureRect(sprite_frames[current_frame]);

    // Reset the Clock
    sprite_clock.restart();
  }

  // Check for movement events
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    move(5.0f, 0.0f);
  }
}

// void Player::draw(const sf::RenderWindow &window) {}

Player::~Player() {}
