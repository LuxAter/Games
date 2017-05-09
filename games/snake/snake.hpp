#ifndef SNAKE_HPP
#define SNAKE_HPP
#include <ostendo.h>
#include <string>
#include <vector>
#include <ncurses.h>

namespace games {
  namespace snake {
    class Snake {
     public:
       Snake(int c = 1, bool ai = true);
       ~Snake();
       void Move();
       void SetDir(int key);
     private:
       void AiMove();

       int grow = 0, rate = 1, color = 1;;
       bool ai_controled = false, dead = false;
       std::pair<int, int> head, move_dir;
       std::vector<std::pair<int, int>> body;
       int controls[4] = {KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT};
    };
    extern std::vector<std::vector<int>> grid, last;
    void Game();
    int Run(ostendo::Window win, int c_obs, int c_food, int c_snakes,
             int growth, int speed);
    std::pair<int, int> FindPos();
    void DisplayGrid(ostendo::Window win);
  }
}
#endif
