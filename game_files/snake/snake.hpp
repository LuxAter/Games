#ifndef SANKE_SNAKES
#define SANKE_SNAKES
#include <vector>

namespace snake {
  class Snake {
   public:
    void Init(int index, int size = 1);
    void SetSpeed(int x, int y);
    void Move();
    void Kill();
    void AI();
    void FindGoal();
    bool alive = true;
    int score = 0, color = 1;

   private:
    int headx, heady, xspeed = 0, yspeed = 0, growsize = 1;
    int grew = 0;
    std::pair<int, int> goal;
    std::vector<std::pair<int, int>> body;
    double Dist(std::pair<int, int> pos);
  };
}
#endif
