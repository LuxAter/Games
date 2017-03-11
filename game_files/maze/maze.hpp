#ifndef MAZE_MAZE_HPP
#define MAZE_MAZE_HPP
#include "grid.hpp"

namespace maze {
  extern Grid grid;
  void Game();
  bool Run(int width, int height);
  void AutoRun(bool fast);
  void Score();
}
#endif
