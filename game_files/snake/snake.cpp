#include <appareo.h>
#include <pessum.h>
#include <algorithm>
#include "snake.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace snake {
  Grid grid(std::make_pair(2, 2));
  std::vector<Snake> snakes;
}

bool snake::SortSnake(const snake::Snake& a, const snake::Snake& b) {
  return (a.score > b.score);
}

void snake::Game() {
  int width = scrwidth - 2;
  int height = scrheight - 6;
  int obsticals = 0;
  int apples = 1;
  int snake_count = 1;
  int growth = 1;
  std::vector<Field> fields;
  Field newfield;
  newfield.name = "Width";
  newfield.type = 1;
  newfield.sval = std::to_string(width);
  fields.push_back(newfield);
  newfield.name = "Height";
  newfield.type = 1;
  newfield.sval = std::to_string(height);
  fields.push_back(newfield);
  newfield.name = "Obstical Count";
  newfield.type = 1;
  newfield.sval = std::to_string(obsticals);
  fields.push_back(newfield);
  newfield.name = "Apple Count";
  newfield.type = 1;
  newfield.sval = std::to_string(apples);
  fields.push_back(newfield);
  newfield.name = "Snakes";
  newfield.type = 1;
  newfield.sval = std::to_string(snake_count);
  fields.push_back(newfield);
  newfield.name = "Grow Size";
  newfield.type = 1;
  newfield.sval = std::to_string(growth);
  fields.push_back(newfield);
  while (Run(width, height, obsticals, apples, snake_count, growth) == true) {
    grid.Delete();
    snakes.clear();
    fields = NewForm(fields, "New Game", scrwidth / 2, 8);
    width = fields[0].ival;
    height = fields[1].ival;
    obsticals = fields[2].ival;
    apples = fields[3].ival;
    snake_count = fields[4].ival;
    growth = fields[5].ival;
    if (width > scrwidth - 2) {
      width = scrwidth - 2;
    }
    if (height > scrheight - 6) {
      height = scrheight - 6;
    }
    if (obsticals > (width * height) / 2) {
      obsticals = (width * height) / 2;
    }
    if (apples > (width * height) / 2) {
      apples = (width * height) / 2;
    }
    if (width < 10) {
      width = 10;
    }
    if (height < 10) {
      height = 10;
    }
    if (obsticals < 0) {
      obsticals = 0;
    }
    if (snake_count < 1) {
      snake_count = 1;
    }
  }
  sort(snakes.begin(), snakes.end(), SortSnake);
  int score_window = windows.size();
  InitializeWindow();
  windows[score_window].CreateWindow("Final Scores", scrwidth / 5,
                                     scrheight / 2, -1, -1, true, true);
  for (int i = 0; i < snakes.size(); i++) {
    std::string str = std::to_string(snakes[i].score);
    std::vector<int> attrs = {};
    int color = int(snakes[i].color_pattern[0]) - 48;
    if (color > 6) {
      color -= 6;
      attrs.push_back(DIM);
    }
    attrs.push_back(color + 12);
    SetAtt(attrs, score_window);
    Print(str, i + 1, ((scrwidth / 5) - str.size()) / 2, score_window);
  }
  while (getch() == ERR) {
  }
  TerminateWindow(score_window);
}

bool snake::Run(int width, int height, int obsticals, int apples,
                int snake_count, int growth) {
  snakes.clear();
  grid = Grid(std::make_pair(width, height));
  grid.GenObs(obsticals);
  grid.SetAppleCount(apples);
  grid.GenApple();
  grid.SetScoreCount(snake_count);
  std::vector<int> color_vec = {0};
  for (int i = 0; i < snake_count; i++) {
    color_vec[0]++;
    for (int j = 0; j < color_vec.size() - 1; j++) {
      if (color_vec[j] == 7) {
        color_vec[j] = 0;
        color_vec[j + 1]++;
      }
    }
    if (color_vec[color_vec.size() - 1] == 7) {
      color_vec[color_vec.size() - 1] = 0;
      color_vec.push_back(0);
    }
    snakes.push_back(Snake(growth, GetColor(color_vec)));
  }
  bool running = true;
  for (int i = 0; i < snakes.size(); i++) {
    snakes[i].Update();
    grid.DisplayScore(snakes[i].score, i, int(snakes[i].color_pattern[0]) - 48);
  }
  grid.Display();
  int in = ERR;
  while (in == ERR) {
    in = getch();
  }
  if (in == KEY_UP) {
    snakes[0].SetDir(std::make_pair(0, -1));
  }
  if (in == KEY_DOWN) {
    snakes[0].SetDir(std::make_pair(0, 1));
  }
  if (in == KEY_LEFT) {
    snakes[0].SetDir(std::make_pair(-1, 0));
  }
  if (in == KEY_RIGHT) {
    snakes[0].SetDir(std::make_pair(1, 0));
  }
  if (in == int(' ')) {
    return (true);
  }
  if (in == int('q')) {
    running = false;
    grid.Delete();
  }
  while (running == true) {
    for (int i = 0; i < snakes.size(); i++) {
      snakes[i].Update();
      grid.DisplayScore(snakes[i].score, i,
                        int(snakes[i].color_pattern[0]) - 48);
    }
    grid.Display();
    for (int i = 1; i < snakes.size(); i++) {
      snakes[i].AI();
    }
    in = getch();
    if (in == KEY_UP) {
      snakes[0].SetDir(std::make_pair(0, -1));
    }
    if (in == KEY_DOWN) {
      snakes[0].SetDir(std::make_pair(0, 1));
    }
    if (in == KEY_LEFT) {
      snakes[0].SetDir(std::make_pair(-1, 0));
    }
    if (in == KEY_RIGHT) {
      snakes[0].SetDir(std::make_pair(1, 0));
    }
    if (in == int('q')) {
      running = false;
      grid.Delete();
    }
    if (in == int(' ')) {
      return (true);
    }
    if (Alive() == false) {
      running = false;
    }
  }
  return (false);
}

bool snake::Alive() {
  bool alive_check = false;
  for (int i = 0; i < snakes.size(); i++) {
    if (snakes[i].alive == true) {
      alive_check = true;
    }
  }
  return (alive_check);
}

std::string snake::GetColor(std::vector<int> color_vec) {
  std::string color_str = "";
  for (int i = 0; i < color_vec.size(); i++) {
    color_str += std::to_string(color_vec[i]);
  }
  return (color_str);
}
