#ifndef SNAKE_GRID
#define SNAKE_GRID
#include <vector>

namespace snake {
  class Grid {
   public:
    void Init(int sizex, int sizey);
    void GenObsticals(int count);
    void SetAppleCount(int count);
    void GenApple();
    void SumGrids();
    void Show();
    bool Empty(int x, int y);
    bool Safe(int x, int y);
    bool Apple(int x, int y);
    void Delete();
    void SetScoreCount(int count);

    void ESnake(int x, int y);
    void SSnake(int x, int y, int val);
    void SObs(int x, int y);
    void Eat(int x, int y);

    void DisplayScore(int score, int index);
    int width, height;
    std::vector<std::pair<int, int>> applepos;

   private:
    int win, scorecount = 1, scorewin;
    int obsticalcount, applecount, currentapples;
    std::vector<std::vector<int>> grid, snakegrid, totalgrid, lasttotal;
  };
}
#endif
