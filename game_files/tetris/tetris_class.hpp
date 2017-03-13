#ifndef TETRIS_TETRIMINO
#define TETRIS_TETRIMINO
#include <vector>
namespace tetris {
  enum Dir { UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4 };
  class Tetrimino {
   public:
    std::pair<int, int> center_pos;
    void Gen();
    void Del();
    bool Move(int dir, int max);
    void Rotate();
    void Display();
    void Erase();
    bool init = false;
    std::vector<int> blocks;

   private:
    int shape = -1, center_pointer = -1;
  };
}
#endif
