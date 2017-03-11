#ifndef SNAKE_SNAKE_CLASS_HPP
#define SNAKE_SNAKE_CLASS_HPP
#include <string>
#include <vector>
namespace snake {
  bool sortfunction(const std::pair<int, int>& i, const std::pair<int, int>& j);
  class Snake {
   public:
    Snake(int growth, std::string color_set = "0");
    void SetDir(std::pair<int, int> new_dir);
    void Update();
    void Kill();
    void AI();
    ~Snake();
    bool alive = true;
    int score = 0;
    std::string color_pattern = "0";
    int color_length = 1;

   private:
    void FindGoal();
    int grow_rate, grew = 0, color_index = 0;
    std::pair<int, int> goal_pos, head, dir;
    std::vector<std::vector<int>> body;
  };
}
#endif
