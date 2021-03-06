// Copyright 2015, Aaron Ceross

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../include/instance_selection.hpp"
#include "../include/resource_manager.hpp"
#include "../include/text_utility.hpp"

InstanceSelection::InstanceSelection(StateStack& stack, Context context)
: State(stack, context)
, options_()
, options_index_(0)
, mapfile_handler_()
{
  sf::Texture& texture = context.textures_->Get(Textures::SelectScreen);
  sf::Font& font = context.fonts_->Get(Fonts::Title);
  background_sprite_.setTexture(texture);

  // Map options
  sf::Text single_variable_option;
  single_variable_option.setFont(font);
  single_variable_option.setString("Single Variable");
  CentreOrigin(single_variable_option);
  single_variable_option.setPosition(context.window_->getView().getSize().x / 1.68f, 210);
  single_variable_option.setCharacterSize(15);
  options_.push_back(single_variable_option);

  sf::Text two_SAT_option;
  two_SAT_option.setFont(font);
  two_SAT_option.setString("2SAT Instance");
  CentreOrigin(two_SAT_option);
  two_SAT_option.setPosition(single_variable_option.getPosition() +
                             sf::Vector2f(0.f, 56.f));
  two_SAT_option.setCharacterSize(15);
  options_.push_back(two_SAT_option);

  sf::Text three_SAT_option;
  three_SAT_option.setFont(font);
  three_SAT_option.setString("3SAT Instance");
  CentreOrigin(three_SAT_option);
  three_SAT_option.setPosition(single_variable_option.getPosition() +
                               sf::Vector2f(0.f, 114.f));
  three_SAT_option.setCharacterSize(15);
  options_.push_back(three_SAT_option);

  sf::Text experimental_option;
  experimental_option.setFont(font);
  experimental_option.setString("Experimental SAT Instance");
  CentreOrigin(experimental_option);
  experimental_option.setPosition(single_variable_option.getPosition() +
                                 sf::Vector2f(10.f, 174.f));
  experimental_option.setCharacterSize(15);
  options_.push_back(experimental_option);

  sf::Text return_option;
  return_option.setFont(font);
  return_option.setString("Return");
  CentreOrigin(return_option);
  return_option.setPosition(single_variable_option.getPosition() +
                            sf::Vector2f(-48.f, 240.f));
  return_option.setCharacterSize(20);
  options_.push_back(return_option);

  sf::Text exit_option;
  exit_option.setFont(font);
  exit_option.setString("Quit");
  CentreOrigin(exit_option);
  exit_option.setPosition(single_variable_option.getPosition() +
                          sf::Vector2f(-55.f, 270.f));
  exit_option.setCharacterSize(20);
  options_.push_back(exit_option);
  UpdateOptionText();
}

void InstanceSelection::Draw() {
  sf::RenderWindow& window = *GetContext().window_;

  window.setView(window.getDefaultView());
  window.draw(background_sprite_);

  for (const sf::Text& text : options_) {
    window.draw(text);
  }
}

bool InstanceSelection::Update(sf::Time) {
  return true;
}

bool InstanceSelection::HandleEvent(const sf::Event& event) {
  if (event.type != sf::Event::KeyPressed) {
    return false;
  }

  if (event.key.code == sf::Keyboard::Return) {
    if (options_index_ == Single) {
      SetMapfile("resources/SAT_instances/single_variable.cnf");
      RequestStackPop();
      RequestStackPush(States::Game);
    } else if (options_index_ == Two) {
      SetMapfile("resources/SAT_instances/simple.cnf");
      RequestStackPop();
      RequestStackPush(States::Game);
    } else if (options_index_ == Three) {
      SetMapfile("resources/SAT_instances/3SAT_Mario_Ben.cnf");
      RequestStackPop();
      RequestStackPush(States::Game);
    } else if (options_index_ == Unsolve) {
      SetMapfile("resources/SAT_instances/quinn.cnf");
      RequestStackPop();
      RequestStackPush(States::Game);
    } else if (options_index_ == Menu) {
      RequestStackPop();
      RequestStackPush(States::Menu);
    } else if (options_index_ == Exit) {
      RequestStackPop();
    }

  } else if (event.key.code == sf::Keyboard::Up) {
    // Decrement and wrap-around
    if (options_index_ > 0)
      --options_index_;
    else
      options_index_ = options_.size() - 1;

    UpdateOptionText();
  } else if (event.key.code == sf::Keyboard::Down) {
    if (options_index_ < options_.size() - 1)
      ++options_index_;
    else
      options_index_ = 0;

    UpdateOptionText();
  }
  return true;
}

void InstanceSelection::UpdateOptionText() {
  if (options_.empty())
    return;

  // White all texts
  for (sf::Text& text : options_)
    text.setColor(sf::Color::White);

  // Yellow the selected text
  options_[options_index_].setColor(sf::Color::Yellow);
}
