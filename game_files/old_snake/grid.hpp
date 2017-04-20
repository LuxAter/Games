#ifndef SNAKE_GRID_HPP
#define SNAKE_GRID_HPP
#include <vector>
#include "snake_class.hpp"
namespace snake {
  class Grid {
   public:
    Grid(std::pair<int, int> size);
    void GenObs(int count);
    void SetAppleCount(int count);
    void SetScoreCount(int count);
    void GenApple();
    void Display();
    void DisplayScore(int score, int index, int color);
    int GetPos(std::pair<int, int> pos);
    void SetPos(std::pair<int, int> pos, int val);
    std::pair<int, int> FindFree();
    void Eat(std::pair<int, int> pos);
    void Delete();
    ~Grid();
    std::pair<int, int> grid_size;
    std::vector<std::pair<int, int>> apple_pos;
    int current_apples = 0;

   private:
    int win = 0, score_win = 0, score_count = 1;
    int apple_count = 0;
    std::vector<std::vector<int>> grid, last_grid;
  };
}
#endif
