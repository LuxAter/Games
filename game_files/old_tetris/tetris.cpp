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
  std::vector<std::vector<int>> grid, last_grid;
  std::vector<int> win;
  int score = -1, level = -1, next_shape = -1, line_count = -1;
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
  new_field.name = "Level";
  new_field.type = 1;
  new_field.sval = "0";
  fields.push_back(new_field);
  level = 1;
  while (Run(width, height) == true) {
    fields = NewForm(fields, "New Game", scrwidth / 2, 5);
    width = fields[0].ival;
    height = fields[1].ival;
    level = fields[2].ival;
    if ((width * 2) + 14 >= scrwidth) {
      width = (scrwidth - 14) / 2;
    }
    if (height + 5 >= scrheight) {
      height = (scrheight - 5);
    }
    if (width <= 5) {
      width = 5;
    }
    if (height < 5) {
      height = 5;
    }
  }
}

bool tetris::Run(int width, int height) {
  score = 0;
  next_shape = rand() % 7;
  line_count = 0;
  InitWindows(width, height);
  grid = std::vector<std::vector<int>>(width, std::vector<int>(height + 2, 0));
  last_grid = grid;
  Tetrimino tetrim;
  bool running = true, return_val = false;
  int tic_delay = 500, tic_count = 0, start_level = 1;
  while (running == true) {
    tic_count++;
    if (tetrim.init == false) {
      tetrim.Gen(next_shape);
      next_shape = rand() % 7;
      DisplayNext(next_shape);
    }
    tetrim.Display();
    DisplayGrid();
    DisplayStats();
    tetrim.Erase();
    if (tic_count > (tic_delay - (level * 5))) {
      if (tetrim.Move(DOWN, height + 1) == true) {
        if (tetrim.center_pos.second > 2) {
          score += 5;
          tetrim.Del();
          CheckLine();
          if (line_count >= (start_level * 10)) {
            level++;
            start_level++;
          }
        } else {
          running = false;
        }
      }
      tic_count = 0;
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
    } else if (in == KEY_UP) {
      tetrim.Rotate(grid.size(), grid[0].size());
    } else if (in == KEY_DOWN) {
      tic_count = tic_delay;
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
          SetAtt({YELLOW_BACK}, win[1]);
        } else if (grid[i][j] == 4) {
          SetAtt({WHITE_BACK}, win[1]);
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

void tetris::DisplayStats() {
  PrintZ(std::to_string(score), 5, win[0]);
  PrintZ(std::to_string(level), 5, win[3]);
  PrintZ(std::to_string(line_count), 5, win[4]);
}

void tetris::DisplayNext(int shape) {
  Tetrimino next_tet;
  next_tet.Gen(shape);
  shape++;
  windows[win[2]].Clear();
  if (shape == 1) {
    SetAtt({CYAN_BACK}, win[2]);
  } else if (shape == 2) {
    SetAtt({BLUE_BACK}, win[2]);
  } else if (shape == 3) {
    SetAtt({YELLOW_BACK}, win[2]);
  } else if (shape == 4) {
    SetAtt({WHITE_BACK}, win[2]);
  } else if (shape == 5) {
    SetAtt({GREEN_BACK}, win[2]);
  } else if (shape == 6) {
    SetAtt({MAGENTA_BACK}, win[2]);
  } else if (shape == 7) {
    SetAtt({RED_BACK}, win[2]);
  }
  for (int i = 0; i < next_tet.blocks.size(); i += 2) {
    Print("  ", (5 + next_tet.blocks[i + 1]) + 1,
          (5 + (next_tet.blocks[i]) * 2), win[2], false);
  }
  SetAtt({NORMAL}, win[2]);
  windows[win[2]].Update();
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
  win.push_back(windows.size());
  InitializeWindow();
  windows[win.back()].CreateWindow(
      "LINES", 12, 3, ((scrwidth - total_width) / 2) + (width * 2) + 2,
      ((scrheight - total_height) / 2) + 18, true, true);
}

void tetris::EraseWindows() {
  for (int i = win.size() - 1; i >= 0; i--) {
    TerminateWindow(win[i]);
    win.erase(win.begin() + i);
  }
}

void tetris::CheckLine() {
  int count = 0;
  for (int i = grid[0].size() - 1; i >= 0; i--) {
    bool clear = true;
    for (int j = 0; j < grid.size(); j++) {
      if (grid[j][i] == 0) {
        clear = false;
      }
    }
    if (clear == true) {
      count++;
      for (int j = 0; j < grid.size(); j++) {
        grid[j][i] = 0;
      }
      for (int j = i - 1; j >= 0; j--) {
        for (int k = 0; k < grid.size(); k++) {
          grid[k][j + 1] = grid[k][j];
        }
      }
      i++;
    }
  }
  if (count == 1) {
    score += 40 * level;
  } else if (count == 2) {
    score += 100 * level;
  } else if (count == 3) {
    score += 300 * level;
  } else if (count == 4) {
    score += 1200 * level;
  }
  line_count += count;
}
