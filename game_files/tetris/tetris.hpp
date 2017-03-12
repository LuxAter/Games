#ifndef TETRIS_TETRIS_HPP
#define TETRIS_TETRIS_HPP
#include <vector>
namespace tetris {
  struct Tetrimino {
    std::pair<int, int> blocks[4];
    int index = 0;
    void Gen();
    void MoveDown();
    void Turn();
  };
  extern int grid_win, next_win, level_win, score_win;
  extern std::vector<std::vector<int>> grid, last_grid;
  void Game();
  bool Run(int width, int height);
  void DisplayData(int next, int level, int score);
  void DisplayGrid();
  void AddTet(int tet);
  void MoveDown();
  void InitWindows(int width, int height);
  void TermWindows();
}
#endif
