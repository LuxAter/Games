#include <appareo.h>
#include <pessum.h>
#include <algorithm>
#include <vector>
#include "game.hpp"
#include "grid.hpp"
#include "snake.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

bool sortfunction(const std::pair<int, int>& i, const std::pair<int, int>& j) {
  return (i.second < j.second);
}

void snake::Snake::Init(int index, int size) {
  bool search = true;
  while (search == true) {
    int x = rand() % snake::grid.width;
    int y = rand() % snake::grid.height;
    if (snake::grid.Empty(x, y) == true) {
      search = false;
      headx = x;
      heady = y;
      alive = true;
      color = index;
      growsize = size;
    }
  }
}

void snake::Snake::SetSpeed(int x, int y) {
  xspeed = x;
  yspeed = y;
}

void snake::Snake::Move() {
  if (alive == true) {
    score += 1;
    snake::grid.DisplayScore(score, color);
    if (grew > 0) {
      body.insert(body.begin(), std::pair<int, int>(headx, heady));
      grew--;
    } else {
      if (body.size() > 0) {
        snake::grid.ESnake(body[body.size() - 1].first,
                           body[body.size() - 1].second);
        for (int i = body.size() - 1; i > 0; i--) {
          body[i].first = body[i - 1].first;
          body[i].second = body[i - 1].second;
        }
        body[0].first = headx;
        body[0].second = heady;
      } else {
        snake::grid.ESnake(headx, heady);
      }
    }
    headx += xspeed;
    heady += yspeed;
    if (headx >= snake::grid.width || headx < 0 ||
        heady >= snake::grid.height || heady < 0) {
      Kill();
    } else {
      if (snake::grid.Safe(headx, heady) == false) {
        Kill();
      } else {
        if (snake::grid.Apple(headx, heady) == true) {
          grew += growsize;
          score += 100;
          snake::grid.Eat(headx, heady);
        }
        snake::grid.SSnake(headx, heady, color);
      }
    }
  }
}

void snake::Snake::Kill() {
  for (int i = 0; i < body.size(); i++) {
    snake::grid.SObs(body[i].first, body[i].second);
    snake::grid.ESnake(body[i].first, body[i].second);
  }
  alive = false;
  body.clear();
}

void snake::Snake::AI() {
  FindGoal();
  std::vector<std::pair<int, int>> vals;
  if (snake::grid.Safe(headx, heady - 1)) {
    vals.push_back(std::pair<int, int>(
        1, abs(headx - goal.first) + abs(heady - 1 - goal.second)));
  }

  if (snake::grid.Safe(headx + 1, heady)) {
    vals.push_back(std::pair<int, int>(
        2, abs(headx + 1 - goal.first) + abs(heady - goal.second)));
  }

  if (snake::grid.Safe(headx, heady + 1)) {
    vals.push_back(std::pair<int, int>(
        3, abs(headx - goal.first) + abs(heady + 1 - goal.second)));
  }

  if (snake::grid.Safe(headx - 1, heady)) {
    vals.push_back(std::pair<int, int>(
        4, abs(headx - 1 - goal.first) + abs(heady - goal.second)));
  }
  sort(vals.begin(), vals.end(), sortfunction);
  for (int i = vals.size() - 1; i >= 0; i--) {
    if (vals[i].second != vals[0].second) {
      vals.pop_back();
    }
  }
  if (vals.size() > 0) {
    int pick = rand() % vals.size();
    if (vals[pick].first == 1) {
      SetSpeed(0, -1);
    }
    if (vals[pick].first == 2) {
      SetSpeed(1, 0);
    }
    if (vals[pick].first == 3) {
      SetSpeed(0, 1);
    }
    if (vals[pick].first == 4) {
      SetSpeed(-1, 0);
    }
  }
}

void snake::Snake::FindGoal() {
  bool find = false;
  if (goal.first == -1) {
    find = true;
  }
  if (find != true && snake::grid.Apple(goal.first, goal.second) == false) {
    find = true;
  }
  if (find == true) {
    goal.first = -1;
    goal.second = -1;
    int minindex = -1;
    double minval = std::numeric_limits<double>::infinity();
    for (int i = 0; i < snake::grid.applepos.size(); i++) {
      if (minval > Dist(snake::grid.applepos[i])) {
        minval = Dist(snake::grid.applepos[i]);
        minindex = i;
      }
    }
    if (minindex == -1) {
      goal.first = 5;
      goal.second = 5;
    } else {
      goal.first = snake::grid.applepos[minindex].first;
      goal.second = snake::grid.applepos[minindex].second;
      pessum::logging::Log("data", "(" + std::to_string(goal.first) + "," + std::to_string(goal.second) + ")");
    }
  }
  //goal.first = 100;
  //goal.second = 1;
}

double snake::Snake::Dist(std::pair<int, int> pos) {
  double val = 0;
  val = sqrt(pow((double)(pos.first - headx), 2) + pow((double)(pos.second - heady), 2));
  return (val);
}
