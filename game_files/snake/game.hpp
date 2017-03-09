#ifndef SNAKE_SNAKE
#define SNAKE_SNAKE

#include "grid.hpp"
#include "snake.hpp"

namespace snake {
  extern Grid grid;
  extern std::vector<Snake> snakes;
  void Game();
  bool Run(int width, int height, int obsticals, int apples, int snakec,
           int size);
  bool Alive();
}

#endif
