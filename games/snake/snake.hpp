#ifndef SNAKE_HPP
#define SNAKE_HPP
#include <vector>
#include <string>

namespace games{
  namespace snake{
    class Snake{};
    void Game();
    void Run(int width, int height, int c_obs, int c_food, int c_snakes, int growth, int speed);
    bool Alive();
    bool SortSnake(const Snake& a, Snake& b);
    void SetColor();
    std::string GetColor(std::vector<int> color_vec);
  }
}
#endif
