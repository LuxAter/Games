#ifndef PACMAN_PACMAN_HPP
#define PACMAN_PACMAN_HPP
#include <vector>
namespace pacman {
  extern std::vector<std::vector<int>> grid;
  void Game();
  void LoadGrid();
}
#endif
