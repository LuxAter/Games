#include <appareo.h>
#include <pessum.h>
#include <vector>
#include "tetris.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace tetris {
  int grid_win, next_win, level_win, score_win;
  std::vector<std::vector<int>> grid, last_grid;
}

void tetris::Tetrimino::Gen() {
  index = (rand() % 7) + 1;
  int grid_center = grid.size() / 2;
  if (index == 1) {
    // ####
    //
    blocks[0] = std::make_pair(grid_center - 2, 1);
    blocks[1] = std::make_pair(grid_center - 1, 1);
    blocks[2] = std::make_pair(grid_center, 1);
    blocks[3] = std::make_pair(grid_center + 1, 1);
  } else if (index == 2) {
    // ###
    //   #
    blocks[0] = std::make_pair(grid_center - 1, 0);
    blocks[1] = std::make_pair(grid_center, 0);
    blocks[2] = std::make_pair(grid_center + 1, 0);
    blocks[3] = std::make_pair(grid_center + 1, 1);
  } else if (index == 3) {
    // ###
    // #
    blocks[0] = std::make_pair(grid_center - 1, 0);
    blocks[1] = std::make_pair(grid_center, 0);
    blocks[2] = std::make_pair(grid_center + 1, 0);
    blocks[3] = std::make_pair(grid_center - 1, 1);
  } else if (index == 4) {
    // ##
    // ##
    blocks[0] = std::make_pair(grid_center - 1, 0);
    blocks[1] = std::make_pair(grid_center - 1, 1);
    blocks[2] = std::make_pair(grid_center, 0);
    blocks[3] = std::make_pair(grid_center, 1);
  } else if (index == 5) {
    //  ##
    // ##
    blocks[0] = std::make_pair(grid_center - 1, 1);
    blocks[1] = std::make_pair(grid_center, 1);
    blocks[2] = std::make_pair(grid_center, 0);
    blocks[3] = std::make_pair(grid_center + 1, 0);
  } else if (index == 6) {
    // ###
    //  #
    blocks[0] = std::make_pair(grid_center - 1, 0);
    blocks[1] = std::make_pair(grid_center, 0);
    blocks[2] = std::make_pair(grid_center + 1, 0);
    blocks[3] = std::make_pair(grid_center, 1);
  } else if (index == 7) {
    // ##
    //  ##
    blocks[0] = std::make_pair(grid_center - 1, 0);
    blocks[1] = std::make_pair(grid_center, 0);
    blocks[2] = std::make_pair(grid_center, 1);
    blocks[3] = std::make_pair(grid_center + 1, 1);
  }
  for (int i = 0; i < 4; i++) {
    grid[blocks[i].first][blocks[i].second] = index;
  }
}

void tetris::Tetrimino::MoveDown() {}

void tetris::Tetrimino::Turn() {}

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
    fields = NewForm(fields, "New Game", scrwidth / 2, 6);
    width = fields[0].ival;
    height = fields[1].ival;
    if (width >= scrwidth) {
      width = (scrwidth - 1);
    }
    if (height >= scrheight) {
      height = (scrheight - 1);
    }
  }
}

bool tetris::Run(int width, int height) {
  bool running = true;
  int level = 1, score = 0, next_tet = 0, current_tet = 0;
  InitWindows(width + 2, height + 2);
  grid = std::vector<std::vector<int>>(width, std::vector<int>(height + 2, 0));
  last_grid = grid;
  while (running == true) {
    if (current_tet == 0) {
      current_tet = (rand() % 7) + 1;
      AddTet(current_tet);
    }
    DisplayData(next_tet, level, score);
    DisplayGrid();
    MoveDown();
    int in = getch();
    if (in == int(' ')) {
      TermWindows();
      return (true);
    } else if (in == 'q') {
      running = false;
    }
  }
  TermWindows();
  return (false);
}

void tetris::DisplayData(int next, int level, int score) {
  PrintZ(std::to_string(level), 6, level_win);
  PrintZ(std::to_string(score), 6, score_win);
}

void tetris::InitWindows(int width, int height) {
  int total_width = width + 10;
  grid_win = windows.size();
  InitializeWindow();
  windows[grid_win].CreateWindow("TETRIS", width, height,
                                 (scrwidth - total_width) / 2, -1, true, true);
  next_win = windows.size();
  InitializeWindow();
  windows[next_win].CreateWindow("Next", 10, 8,
                                 (scrwidth - total_width) / 2 + width,
                                 (scrheight - height) / 2, true, true);
  level_win = windows.size();
  InitializeWindow();
  windows[level_win].CreateWindow("Level", 10, 3,
                                  (scrwidth - total_width) / 2 + width,
                                  (scrheight - height) / 2 + 8, true, true);
  score_win = windows.size();
  InitializeWindow();
  windows[score_win].CreateWindow("Score", 10, 3,
                                  (scrwidth - total_width) / 2 + width,
                                  (scrheight - height) / 2 + 11, true, true);
  // Grid size = 10x22 with top two hidden;
}

void tetris::DisplayGrid() {
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 2; j < grid[i].size(); j++) {
      if (grid[i][j] != last_grid[i][j]) {
        last_grid[i][j] = grid[i][j];
        if (grid[i][j] == 1) {
          SetAtt({CYAN_BACK}, grid_win);
        } else if (grid[i][j] == 2) {
          SetAtt({BLUE_BACK}, grid_win);
        } else if (grid[i][j] == 3) {
          SetAtt({YELLOW_BACK, DIM}, grid_win);
        } else if (grid[i][j] == 4) {
          SetAtt({YELLOW_BACK}, grid_win);
        } else if (grid[i][j] == 5) {
          SetAtt({GREEN_BACK}, grid_win);
        } else if (grid[i][j] == 6) {
          SetAtt({MAGENTA_BACK}, grid_win);
        } else if (grid[i][j] == 7) {
          SetAtt({RED_BACK}, grid_win);
        }
        Print(" ", j - 1, i + 1, grid_win, false);
        SetAtt({NORMAL}, grid_win);
      }
    }
  }
  windows[grid_win].Update();
}

void tetris::AddTet(int tet) {
  int grid_center = grid.size() / 2;
  if (tet == 1) {
    grid[grid_center - 2][1] = tet;
    grid[grid_center - 1][1] = tet;
    grid[grid_center][1] = tet;
    grid[grid_center + 1][1] = tet;
  } else if (tet == 2) {
    grid[grid_center - 1][0] = tet;
    grid[grid_center][0] = tet;
    grid[grid_center + 1][0] = tet;
    grid[grid_center + 1][1] = tet;
  } else if (tet == 3) {
    grid[grid_center - 1][0] = tet;
    grid[grid_center][0] = tet;
    grid[grid_center + 1][0] = tet;
    grid[grid_center - 1][1] = tet;
  } else if (tet == 4) {
    grid[grid_center - 1][0] = tet;
    grid[grid_center][0] = tet;
    grid[grid_center][1] = tet;
    grid[grid_center - 1][1] = tet;
  } else if (tet == 5) {
    grid[grid_center - 1][1] = tet;
    grid[grid_center][0] = tet;
    grid[grid_center][1] = tet;
    grid[grid_center + 1][0] = tet;
  } else if (tet == 6) {
    grid[grid_center - 1][0] = tet;
    grid[grid_center][0] = tet;
    grid[grid_center + 1][0] = tet;
    grid[grid_center][1] = tet;
  } else if (tet == 7) {
    grid[grid_center + 1][1] = tet;
    grid[grid_center][0] = tet;
    grid[grid_center][1] = tet;
    grid[grid_center - 1][0] = tet;
  }
}

void tetris::MoveDown() {
  for (int i = 0; i < grid.size(); i++) {
    for (int j = grid[i].size() - 2; j >= 0; j--) {
      if (grid[i][j] != 0 && grid[i][j + 1] == 0) {
        grid[i][j + 1] = grid[i][j];
        grid[i][j] = 0;
      }
    }
  }
}

void tetris::TermWindows() {
  TerminateWindow(score_win);
  TerminateWindow(level_win);
  TerminateWindow(next_win);
  TerminateWindow(grid_win);
}
