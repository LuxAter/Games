#include <appareo.h>
#include <pessum.h>
#include <vector>
#include "tetris.hpp"
#include "tetris_class.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace tetris {
  // std::vector<Tetrimino> tetriminos;
  std::vector<std::vector<int>> grid, last_grid;
  std::vector<int> win;
}

void tetris::Game() {
  int width = 10, height = 20;
  std::vector<Field> fields;
  Field new_field;
  new_field.name = "Width";
  new_field.type = 1;
  new_field.sval = std::to_string(width);
  fields.push_back(new_field);
  new_field.name = "Height";
  new_field.type = 1;
  new_field.sval = std::to_string(height);
  fields.push_back(new_field);
  while (Run(width, height) == true) {
    fields = NewForm(fields, "New Game", scrwidth / 2, 4);
    width = fields[0].ival;
    height = fields[1].ival;
    if ((width * 2) + 14 >= scrwidth) {
      width = (scrwidth - 14) / 2;
    }
    if (height + 5 >= scrheight) {
      height = (scrheight - 5);
    }
  }
}

bool tetris::Run(int width, int height) {
  InitWindows(width, height);
  grid = std::vector<std::vector<int>>(width, std::vector<int>(height + 2, 0));
  last_grid = grid;
  Tetrimino tetrim;
  bool running = true, return_val = false;
  while (running == true) {
    if (tetrim.init == false) {
      tetrim.Gen();
    }
    tetrim.Display();
    DisplayGrid();
    tetrim.Erase();
    if (tetrim.Move(DOWN, height + 1) == true) {
      tetrim.Del();
    }
    int in = getch();
    if (in == int(' ')) {
      running = false;
      return_val = true;
    } else if (in == int('q')) {
      running = false;
      return_val = false;
    } else if (in == KEY_RIGHT) {
      tetrim.Move(RIGHT, width - 1);
    } else if (in == KEY_LEFT) {
      tetrim.Move(LEFT, 0);
    } else if (in == KEY_DOWN) {
      tetrim.Move(DOWN, height + 1);
    } else if (in == KEY_UP) {
      tetrim.Rotate();
    }
  }
  tetrim.Del();
  grid.clear();
  last_grid.clear();
  EraseWindows();
  return (return_val);
}

void tetris::DisplayGrid() {
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 2; j < grid[i].size(); j++) {
      if (grid[i][j] != last_grid[i][j]) {
        last_grid[i][j] = grid[i][j];
        if (grid[i][j] == 1) {
          SetAtt({CYAN_BACK}, win[1]);
        } else if (grid[i][j] == 2) {
          SetAtt({BLUE_BACK}, win[1]);
        } else if (grid[i][j] == 3) {
          SetAtt({YELLOW_BACK, DIM}, win[1]);
        } else if (grid[i][j] == 4) {
          SetAtt({YELLOW_BACK}, win[1]);
        } else if (grid[i][j] == 5) {
          SetAtt({GREEN_BACK}, win[1]);
        } else if (grid[i][j] == 6) {
          SetAtt({MAGENTA_BACK}, win[1]);
        } else if (grid[i][j] == 7) {
          SetAtt({RED_BACK}, win[1]);
        }
        Print("  ", j - 1, (i * 2) + 1, win[1], false);
        SetAtt({NORMAL}, win[1]);
      }
    }
  }
  windows[win[1]].Update();
}

void tetris::InitWindows(int width, int height) {
  int total_width = (width * 2) + 2 + 12;
  int total_height = height + 2 + 3;
  win.push_back(windows.size());
  InitializeWindow();
  windows[win.back()].CreateWindow("TETRIS", total_width, 3, -1,
                                   (scrheight - total_height) / 2, true, true);
  win.push_back(windows.size());
  InitializeWindow();
  windows[win.back()].CreateWindow(
      "grid", (width * 2) + 2, height + 2, (scrwidth - total_width) / 2,
      ((scrheight - total_height) / 2) + 3, true, false);
  win.push_back(windows.size());
  InitializeWindow();
  windows[win.back()].CreateWindow(
      "NEXT", 12, 12, ((scrwidth - total_width) / 2) + (width * 2) + 2,
      ((scrheight - total_height) / 2) + 3, true, true);
  win.push_back(windows.size());
  InitializeWindow();
  windows[win.back()].CreateWindow(
      "LEVEL", 12, 3, ((scrwidth - total_width) / 2) + (width * 2) + 2,
      ((scrheight - total_height) / 2) + 15, true, true);
}

void tetris::EraseWindows() {
  for (int i = win.size() - 1; i >= 0; i--) {
    TerminateWindow(win[i]);
    win.erase(win.begin() + i);
  }
}
