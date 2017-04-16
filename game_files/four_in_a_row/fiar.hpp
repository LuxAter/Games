#ifndef FOUR_IN_A_ROW_FIAR_HPP
#define FOUR_IN_A_ROW_FIAR_HPP
#include <vector>
#include <ostendo.h>
namespace fiar {
  extern std::vector<std::vector<int>> grid;
  void Game();
  bool Run(int width, int height, int win_length, bool ai);
  void DrawGrid(int width, int height, ostendo::Window win, int pos);
  void Pause(int tics);
  bool Win(int player, int length, bool real = true);
  bool CatsGame();
  int AI(int length);
  int CanWin(int player, int length);
}
#endif
