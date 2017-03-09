#ifndef MAZE_MAZE
#define MAZE_MAZE
#include "grid.hpp"

namespace maze {
  extern Grid grid;
  void Game();
  bool Run(int width, int height);
  void AutoRun(bool fast);
  void Score();
}
#endif
