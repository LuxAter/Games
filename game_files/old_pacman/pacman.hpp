#ifndef PACMAN_PACMAN_HPP
#define PACMAN_PACMAN_HPP
#include <vector>
namespace pacman {
  extern std::vector<std::vector<int>> grid, last_grid;
  extern int win, score_win;
  void Game();
  void DisplayGrid();
  void LoadGrid();
}
#endif
