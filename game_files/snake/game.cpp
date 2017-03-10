#include <appareo.h>
#include <algorithm>
#include <ctime>
#include <vector>
#include "game.hpp"
#include "grid.hpp"
#include "snake.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace snake {
  Grid grid;
  std::vector<Snake> snakes;
}

bool SortSnake(const snake::Snake& a, const snake::Snake& b) {
  return (a.score > b.score);
}

void snake::Game() {
  snakes.clear();
  int width = scrwidth - 2;
  int height = scrheight - 6;
  int obsticals = 0;
  int apples = 1;
  int snakec = 1;
  int size = 5;
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
  newfield.sval = std::to_string(snakec);
  fields.push_back(newfield);
  newfield.name = "Grow Size";
  newfield.type = 1;
  newfield.sval = std::to_string(size);
  fields.push_back(newfield);
  while (Run(width, height, obsticals, apples, snakec, size) == true) {
    grid.Delete();
    for (int i = 0; i < snakes.size(); i++) {
      snakes[i].Kill();
    }
    snakes.clear();
    fields = NewForm(fields, "New Game", scrwidth / 2, 8);
    width = fields[0].ival;
    height = fields[1].ival;
    obsticals = fields[2].ival;
    apples = fields[3].ival;
    snakec = fields[4].ival;
    size = fields[5].ival;
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
    if (apples < 1) {
      apples = 1;
    }
    if (snakec < 1) {
      snakec = 1;
    }
  }
  sort(snakes.begin(), snakes.end(), SortSnake);
  int scorewindow = windows.size();
  InitializeWindow();
  windows[scorewindow].CreateWindow("Final Scores", scrwidth / 5, scrheight / 2,
                                    -1, -1, true, true);
  for (int i = 0; i < snakes.size(); i++) {
    std::string str = std::to_string(snakes[i].score);
    if (snakes[i].color == 1) {
      SetAtt({WHITE_TEXT}, scorewindow);
    } else if (snakes[i].color == 2) {
      SetAtt({BLUE_TEXT}, scorewindow);
    } else if (snakes[i].color == 3) {
      SetAtt({RED_TEXT}, scorewindow);
    } else if (snakes[i].color == 4) {
      SetAtt({YELLOW_TEXT}, scorewindow);
    } else if (snakes[i].color == 5) {
      SetAtt({GREEN_TEXT}, scorewindow);
    } else if (snakes[i].color == 6) {
      SetAtt({CYAN_BACK}, scorewindow);
    } else if(snakes[i].color == 7){
      SetAtt({MAGENTA_BACK}, scorewindow);
    }
    Print(str, i + 1, ((scrwidth / 5) - str.size()) / 2, scorewindow);
  }
  while (getch() == ERR) {
  }
  TerminateWindow(scorewindow);
}

bool snake::Run(int width, int height, int obsticals, int apples, int snakec,
                int size) {
  int ticcount = CLOCKS_PER_SEC / 100;
  // int ticcount = 0;
  grid.Init(width, height);
  grid.GenObsticals(obsticals);
  grid.SetAppleCount(apples);
  grid.SetScoreCount(snakec);
  Snake newsnake;
  for (int i = 0; i < snakec; i++) {
    newsnake.Init(i + 1, size);
    snakes.push_back(newsnake);
  }
  bool running = true;
  for (int i = 0; i < snakes.size(); i++) {
    snakes[i].Move();
  }
  grid.Show();
  int in = ERR;
  while (in == ERR) {
    in = getch();
  }
  if (in == KEY_UP) {
    snakes[0].SetSpeed(0, -1);
  }
  if (in == KEY_DOWN) {
    snakes[0].SetSpeed(0, 1);
  }
  if (in == KEY_LEFT) {
    snakes[0].SetSpeed(-1, 0);
  }
  if (in == KEY_RIGHT) {
    snakes[0].SetSpeed(1, 0);
  }
  if (in == int(' ')) {
    return (true);
  }
  while (running == true) {
    clock_t start = clock();
    for (int i = 0; i < snakes.size(); i++) {
      snakes[i].Move();
    }
    grid.Show();
    for (int i = 1; i < snakes.size(); i++) {
      snakes[i].AI();
    }
    in = getch();
    if (in == KEY_UP) {
      snakes[0].SetSpeed(0, -1);
    }
    if (in == KEY_DOWN) {
      snakes[0].SetSpeed(0, 1);
    }
    if (in == KEY_LEFT) {
      snakes[0].SetSpeed(-1, 0);
    }
    if (in == KEY_RIGHT) {
      snakes[0].SetSpeed(1, 0);
    }
    if (in == int('q')) {
      running = false;
      grid.Delete();
    }
    if (in == int(' ')) {
      return (true);
    }
    while (clock() < start + ticcount) {
    }
    if (Alive() == false) {
      running = false;
    }
  }
  return (false);
}

bool snake::Alive() {
  bool alivecheck = false;
  for (int i = 0; i < snakes.size(); i++) {
    if (snakes[i].alive == true) {
      alivecheck = true;
    }
  }
  return (alivecheck);
}
