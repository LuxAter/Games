#ifndef MINESW_MINESW
#define MINESW_MINESW
#include "grid.hpp"

namespace minesw {
  extern Grid grid;
  void Game();
  bool Run(int width, int height, int mines);
  void Score();
}
#endif
