#include <pessum.h>
#include <algorithm>
#include <string>
#include <vector>
#include "snake.hpp"
#include "snake_class.hpp"

bool snake::sortfunction(const std::pair<int, int>& i,
                         const std::pair<int, int>& j) {
  return (i.second < j.second);
}

snake::Snake::Snake(int growth, std::string color_set) {
  color_pattern = color_set;
  color_length = color_pattern.size();
  grow_rate = growth;
  head = grid.FindFree();
}

void snake::Snake::SetDir(std::pair<int, int> new_dir) { dir = new_dir; }

void snake::Snake::Update() {
  if (alive == true) {
    score += 1;
    if (grew > 0) {
      char current_color = color_pattern[color_index];
      color_index++;
      if (color_index >= color_length) {
        color_index = 0;
      }
      std::vector<int> segment = {head.first, head.second,
                                  int(current_color) - 47};
      body.insert(body.begin(), segment);
      grid.SetPos(std::make_pair(body[0][0], body[0][1]), body[0][2]);
      grew--;
    } else {
      if (body.size() > 0) {
        for (int i = 0; i < body.size(); i++) {
          grid.SetPos(std::make_pair(body[i][0], body[i][1]), 0);
        }
        for (int i = body.size() - 1; i > 0; i--) {
          body[i][0] = body[i - 1][0];
          body[i][1] = body[i - 1][1];
          grid.SetPos(std::make_pair(body[i][0], body[i][1]), body[i][2]);
        }
        body[0][0] = head.first;
        body[0][1] = head.second;
        grid.SetPos(std::make_pair(body[0][0], body[0][1]), body[0][2]);
      } else {
        grid.SetPos(head, 0);
      }
    }
    head.first += dir.first;
    head.second += dir.second;
    int pos_val = grid.GetPos(head);
    if (pos_val != 0 && pos_val != -2) {
      Kill();
    } else if (pos_val == -2) {
      grew += grow_rate;
      score += (100 * grow_rate);
      grid.Eat(head);
    }
    if (alive == true) {
      grid.SetPos(head, int(color_pattern[0]) - 47);
    }
  }
}

void snake::Snake::Kill() {
  alive = false;
  for (int i = 0; i < body.size(); i++) {
    grid.SetPos(std::make_pair(body[i][0], body[i][1]), -1);
  }
  body.clear();
}

void snake::Snake::AI() {
  FindGoal();
  std::vector<std::pair<int, int>> vals;
  int pos_val = grid.GetPos(std::make_pair(head.first, head.second - 1));
  if (pos_val == -2 || pos_val == 0) {
    vals.push_back(
        std::pair<int, int>(1, abs(head.first - goal_pos.first) +
                                   abs(head.second - 1 - goal_pos.second)));
  }
  pos_val = grid.GetPos(std::make_pair(head.first + 1, head.second));
  if (pos_val == -2 || pos_val == 0) {
    vals.push_back(
        std::pair<int, int>(2, abs(head.first + 1 - goal_pos.first) +
                                   abs(head.second - goal_pos.second)));
  }
  pos_val = grid.GetPos(std::make_pair(head.first, head.second + 1));
  if (pos_val == -2 || pos_val == 0) {
    vals.push_back(
        std::pair<int, int>(3, abs(head.first - goal_pos.first) +
                                   abs(head.second + 1 - goal_pos.second)));
  }
  pos_val = grid.GetPos(std::make_pair(head.first - 1, head.second));
  if (pos_val == -2 || pos_val == 0) {
    vals.push_back(
        std::pair<int, int>(4, abs(head.first - 1 - goal_pos.first) +
                                   abs(head.second - goal_pos.second)));
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
      SetDir(std::make_pair(0, -1));
    }
    if (vals[pick].first == 2) {
      SetDir(std::make_pair(1, 0));
    }
    if (vals[pick].first == 3) {
      SetDir(std::make_pair(0, 1));
    }
    if (vals[pick].first == 4) {
      SetDir(std::make_pair(-1, 0));
    }
  }
}

snake::Snake::~Snake() { body.clear(); }

void snake::Snake::FindGoal() {
  bool find = false;
  if (goal_pos.first == -1) {
    find = true;
  } else if (grid.GetPos(goal_pos) != -2) {
    find = true;
  }
  if (find == true) {
    goal_pos.first = -1;
    goal_pos.second = -1;
    int minindex = -1;
    double minval = std::numeric_limits<double>::infinity();
    for (int i = 0; i < grid.apple_pos.size(); i++) {
      if (minval >
          sqrt(pow((double)(grid.apple_pos[i].first - head.first), 2) +
               pow((double)(grid.apple_pos[i].second - head.second), 2))) {
        minval = sqrt(pow((double)(grid.apple_pos[i].first - head.first), 2) +
                      pow((double)(grid.apple_pos[i].second - head.second), 2));
        minindex = i;
      }
    }
    if (minindex == -1) {
      goal_pos.first = 5;
      goal_pos.second = 5;
    } else {
      goal_pos.first = snake::grid.apple_pos[minindex].first;
      goal_pos.second = snake::grid.apple_pos[minindex].second;
    }
  }
}
