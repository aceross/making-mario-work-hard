// Copyright 2015, Aaron Ceross

#include "../include/game_state_manager.hpp"
#include "../include/menu_state.hpp"

int main() {
  GameStateManager gsm;

  gsm.pushState(new MenuState(&gsm));
  gsm.gameLoop();
  std::cout << "...Exiting program..." << std::endl;

  return 0;
}
