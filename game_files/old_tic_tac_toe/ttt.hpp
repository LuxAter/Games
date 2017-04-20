#ifndef TIC_TAC_TOE_TTT_HPP
#define TIC_TAC_TOE_TTT_HPP
#include <vector>
namespace ttt {
  extern std::vector<std::vector<int>> grid;
  void Game();
  bool Run(int width, int height, int win_length, bool ai);
  void DrawGrid(int width, int height, int win, std::pair<int, int> pos);
  bool Win(int player, int length, bool real = true);
  std::pair<int, int> AI(int length);
  std::pair<int, int> CanWin(int player, int length);
  bool CatsGame();
}
#endif
