#include <appareo.h>
#include <pessum.h>
#include <vector>
#include "grid.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

snake::Grid::Grid(std::pair<int, int> size) {
  grid = std::vector<std::vector<int>>(size.first,
                                       std::vector<int>(size.second, 0));
  last_grid = grid;
  grid_size = size;
  win = windows.size();
  InitializeWindow();
  windows[win].CreateWindow("Snake", size.first + 2, size.second + 2, -1,
                            (scrheight - size.second + 2) / 2, true, true);
  score_win = windows.size();
  InitializeWindow();
  windows[score_win].CreateWindow("Score", size.first + 2, 3, -1,
                                  (scrheight - size.second + 2) / 2 - 3, true,
                                  true);
}

void snake::Grid::GenObs(int count) {
  while (count > 0) {
    SetPos(FindFree(), -1);
    count--;
  }
}

void snake::Grid::SetAppleCount(int count) { apple_count = count; }

void snake::Grid::SetScoreCount(int count) { score_count = count; }

void snake::Grid::GenApple() {
  while (current_apples < apple_count) {
    std::pair<int, int> new_pos = FindFree();
    apple_pos.push_back(new_pos);
    SetPos(new_pos, -2);
    current_apples++;
  }
}

void snake::Grid::Display() {
  for (int i = 0; i < grid_size.first; i++) {
    for (int j = 0; j < grid_size.second; j++) {
      if (last_grid[i][j] != grid[i][j]) {
        last_grid[i][j] = grid[i][j];
        std::string str = " ";
        if (grid[i][j] == -1) {
          str = "#";
        } else if (grid[i][j] == -2) {
          str = "*";
          SetAtt({GREEN_TEXT}, win);
        }
        if (grid[i][j] > 0) {
          std::vector<int> attrs = {};
          int color = grid[i][j] - 1;
          if (color > 6) {
            color -= 6;
            attrs.push_back(DIM);
          }
          attrs.push_back(color + 20);
          SetAtt(attrs, win);
        }
        Print(str, j + 1, i + 1, win, false);
        if (grid[i][j] == -2 || grid[i][j] > 0) {
          SetAtt({NORMAL}, win);
        }
      }
    }
  }
  windows[win].Update();
}

void snake::Grid::DisplayScore(int score, int index, int color) {
  std::vector<int> attrs = {};
  index++;
  if (color > 6) {
    color -= 6;
    attrs.push_back(DIM);
  }
  attrs.push_back(color + 12);
  SetAtt(attrs, score_win);
  int pos = (index * (grid_size.first / score_count)) -
            (grid_size.first / score_count);
  Print(std::to_string(score), 1, pos + 1, score_win, false);
  SetAtt({NORMAL}, score_win);
  windows[score_win].Update();
}

int snake::Grid::GetPos(std::pair<int, int> pos) {
  if (pos.first < 0 || pos.first >= grid_size.first || pos.second < 0 ||
      pos.second >= grid_size.second) {
    return (-3);
  } else {
    return (grid[pos.first][pos.second]);
  }
  return (0);
}

void snake::Grid::SetPos(std::pair<int, int> pos, int val) {
  if (pos.first >= 0 && pos.first < grid_size.first && pos.second >= 0 &&
      pos.second < grid_size.second) {
    grid[pos.first][pos.second] = val;
  }
}

std::pair<int, int> snake::Grid::FindFree() {
  bool searching = true;
  std::pair<int, int> pos(0, 0);
  while (searching == true) {
    pos.first = rand() % grid_size.first;
    pos.second = rand() % grid_size.second;
    if (GetPos(pos) == 0) {
      searching = false;
    }
  }
  return (pos);
}

void snake::Grid::Eat(std::pair<int, int> pos) {
  for (int i = 0; i < apple_pos.size(); i++)
    if (apple_pos[i] == pos) {
      SetPos(pos, 0);
      apple_pos.erase(apple_pos.begin() + i);
      current_apples--;
      GenApple();
      break;
    }
}

void snake::Grid::Delete() {
  grid.clear();
  last_grid.clear();
  apple_pos.clear();
  TerminateWindow(score_win);
  TerminateWindow(win);
}

snake::Grid::~Grid() {
  // TerminateWindow(score_win);
  // TerminateWindow(win);
}
