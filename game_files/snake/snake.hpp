#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP
#include <vector>
#include "grid.hpp"
#include "snake_class.hpp"
namespace snake {
  extern Grid grid;
  bool SortSnake(const snake::Snake& a, const snake::Snake& b);
  void Game();
  bool Run(int width, int height, int obsticals, int apples, int snake_count,
           int growth);
  bool Alive();
  std::string GetColor(std::vector<int> color_vec);
}
#endif
