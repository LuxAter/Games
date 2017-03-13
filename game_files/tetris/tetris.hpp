#ifndef TETRIS_TETRIS
#define TETRIS_TETRIS
#include <vector>
#include "tetris_class.hpp"
namespace tetris {
  // extern Tetrimino tet;
  extern std::vector<std::vector<int>> grid, last_grid;
  extern std::vector<int> win;
  void Game();
  bool Run(int width, int height);
  void DisplayGrid();
  void InitWindows(int width, int height);
  void EraseWindows();
}
#endif
