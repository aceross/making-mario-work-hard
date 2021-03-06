// Copyright 2015, Aaron Ceross

#ifndef INPUT_STATE_HPP
#define INPUT_STATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

#include "state.hpp"

class InputState : public State {
 public:
  InputState(StateStack& stack, Context context);

  virtual void Draw();
  virtual bool Update(sf::Time dt);
  virtual bool HandleEvent(const sf::Event& event);

  void UpdateOptionText();
  bool GetInput(const sf::Event& event);

 private:
  enum OptionNames { Create, Restart, Return };

 private:
  sf::Sprite background_sprite_;

  std::vector<sf::Text>  options_;
  std::size_t        options_index_;

  bool inputting_vars_;
  bool inputting_clauses_;
};

#endif  // INPUT_STATE_HPP
