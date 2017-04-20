#include <appareo.h>
#include <pessum.h>
#include "pacman.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace pacman {
  std::vector<std::vector<int>> grid, last_grid;
  int win, score_win;
}

void pacman::Game() {
  bool running = true;
  InitializeWindow();
  win = windows.size() - 1;
  windows[win].CreateWindow("grid", 30, 32, -1, (scrheight - 35) / 2 + 3, true,
                            false);
  InitializeWindow();
  score_win = windows.size() - 1;
  windows[score_win].CreateWindow("PACMAN", 30, 3, -1, (scrheight - 35) / 2,
                                  true, true);

  LoadGrid();
  while (running == true) {
    DisplayGrid();
    int in = getch();
    if (in == int('q')) {
      running = false;
    }
  }
  TerminateWindow(score_win);
  TerminateWindow(win);
}

void pacman::DisplayGrid() {
  for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 30; j++) {
      if (last_grid[i][j] != grid[i][j]) {
        last_grid[i][j] = grid[i][j];
        std::string str = " ";
        if(grid[i][j] == 0){
          str = "*";
        } else if(grid[i][j] == 1){
          str = "#";
        } else if(grid[i][j] == 2){
          str = " ";
        } else if(grid[i][j] == 3){
          str = "X";
        }
        Print(str, i + 1, j + 1, win, false);
      }
    }
  }
  windows[win].Update();
}

void pacman::LoadGrid() {
  grid.clear();
  last_grid = std::vector<std::vector<int>>(30, std::vector<int>(28, 0));
  std::ifstream load_grid("pacman_grid.txt");
  if (load_grid.is_open()) {
    std::string in;
    while (getline(load_grid, in)) {
      std::vector<int> row;
      for (int i = 0; i < in.size(); i++) {
        row.push_back(int(in[i]) - 48);
      }
      grid.push_back(row);
    }
  }
}
