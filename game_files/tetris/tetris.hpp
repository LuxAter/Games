#ifndef TETRIS_TETRIS
#define TETRIS_TETRIS
#include <vector>
#include "tetris_class.hpp"
namespace tetris {
  extern std::vector<std::vector<int>> grid, last_grid;
  extern std::vector<int> win;
  extern int score, level, next_shape, line_count, level_count;
  void Game();
  bool Run(int width, int height);
  void DisplayGrid();
  void DisplayStats();
  void InitWindows(int width, int height);
  void EraseWindows();
  void CheckLine();
}
#endif
