#include <ostendo.h>
#include <pessum.h>
#include <vector>
#include "grid.hpp"

using namespace ostendo;

void maze::Grid::Init(int sizex, int sizey, bool winc) {
  dispx = 0;
  dispy = 0;
  lastdispx = -1;
  lastdispy = -1;
  grid.clear();
  score = 0;
  wincheck = winc;
  if (winc == true) {
    if (sizex >= std_scr.w - 2 && sizey >= std_scr.h - 2) {
      win = Window(std_scr.w, std_scr.h - 3, 0, 3);
    } else if (sizex >= std_scr.w - 2 && sizey < std_scr.h - 2) {
      win = Window(std_scr.w, sizey + 2, 0, (std_scr.h - sizey + 2) / 2);
    } else if (sizex < std_scr.w - 2 && sizey >= std_scr.h - 2) {
      win = Window(sizex + 2, std_scr.h - 3, (std_scr.w - (sizex + 2)) / 2, 3);
    } else {
      win = Window(sizex + 2, sizey + 2, (std_scr.w - (sizex + 2)) / 2,
                   (std_scr.h - (sizey + 2)) / 2);
    }
    win.ToggleBorder();
    win.ToggleTitle("Maze");
    count_win = Window(std_scr.w, 3, 0, 0);
    count_win.ToggleBorder();
    count_win.ToggleTitle("Move Count");

    std::vector<std::vector<int>> dispgrid(
        win.window_space.w, std::vector<int>(win.window_space.h, -5));
    lastdispgrid = dispgrid;
  }
  width = sizex;
  height = sizey;
  GenGrid();
}

bool maze::Grid::CheckPos(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    if (grid[x][y] == 1) {
      return (1);
    }
  }
  return (0);
}

void maze::Grid::Show() {
  if (dispx != lastdispx || dispy != lastdispy) {
    std::vector<std::vector<int>> dispgrid(
        win.window_space.w, std::vector<int>(win.window_space.h, 0));
    for (int i = dispx; i < dispx + win.window_space.w; i++) {
      for (int j = dispy; j < dispy + win.window_space.h; j++) {
        dispgrid[i - dispx][j - dispy] = grid[i][j];
        if (i == width - 2 && j == height - 2) {
          dispgrid[i - dispx][j - dispy] = 2;
        }
      }
    }
    for (int i = 0; i < dispgrid.size(); i++) {
      for (int j = 0; j < dispgrid[0].size(); j++) {
        std::string str = " ";
        if (dispgrid[i][j] != lastdispgrid[i][j]) {
          if (dispgrid[i][j] == 0) {
            win.ColorOn(77);
          } else if (dispgrid[i][j] == 1) {
            win.ColorOn(70);
          } else if (dispgrid[i][j] == 2) {
            win.ColorOn(72);
          } else if (dispgrid[i][j] == 3) {
            win.ColorOn(74);
          }
          win.Print(str, j + 1, i + 1);
          if (dispgrid[i][j] == 0) {
            win.ColorOff(77);
          } else if (dispgrid[i][j] == 1) {
            win.ColorOff(70);
          } else if (dispgrid[i][j] == 2) {
            win.ColorOff(72);
          } else if (dispgrid[i][j] == 3) {
            win.ColorOff(74);
          }
        }
      }
    }
    win.Update();
    lastdispx = dispx;
    lastdispy = dispy;
  }
  count_win.Print("#c%i", score);
}

void maze::Grid::GenGrid() {
  std::vector<std::vector<int>> falsegrid(width, std::vector<int>(height, 0));
  grid = falsegrid;
  std::vector<std::pair<int, int>> stack;
  std::pair<int, int> startcell;
  startcell.first = 1;
  startcell.second = 1;
  int x = 1;
  int y = 1;
  stack.push_back(startcell);
  while (stack.size() > 0) {
    grid[x][y] = 1;
    std::vector<int> check;
    if (x > 1 && grid[x - 2][y] == 0) {
      check.push_back(1);
    }
    if (x < width - 3 && grid[x + 2][y] == 0) {
      check.push_back(2);
    }
    if (y > 1 && grid[x][y - 2] == 0) {
      check.push_back(3);
    }
    if (y < height - 3 && grid[x][y + 2] == 0) {
      check.push_back(4);
    }
    if (check.size() > 0) {
      std::pair<int, int> newcell;
      newcell.first = x;
      newcell.second = y;
      stack.push_back(newcell);
      int direction = check[rand() % check.size()];
      if (direction == 1) {
        grid[x - 1][y] = 1;
        x = x - 2;
      }
      if (direction == 2) {
        grid[x + 1][y] = 1;
        x = x + 2;
      }
      if (direction == 3) {
        grid[x][y - 1] = 1;
        y = y - 2;
      }
      if (direction == 4) {
        grid[x][y + 1] = 1;
        y = y + 2;
      }
    } else if (check.size() == 0) {
      x = stack.back().first;
      y = stack.back().second;
      stack.pop_back();
    }
  }
}

void maze::Grid::Delete() {
  grid.clear();
  score = 0;
  lastdispgrid.clear();
  dispx = 0;
  dispy = 0;
  lastdispy = -1;
  lastdispx = -1;
  width = 0;
  height = 0;
  if (wincheck == true) {
    count_win.DelWin();
    win.DelWin();
  }
}

void maze::Grid::ShowPos(int x, int y) {
  win.ColorOn(74);
  win.Print(" ", y + 1 - dispy, x + 1 - dispx);
  win.ColorOff(74);
}

void maze::Grid::SetDisp(int x, int y) {
  if (x - dispx > (win.window_space.w / 2) + 10) {
    dispx += 1;
  }
  if (y - dispy > (win.window_space.h / 2) + 10) {
    dispy += 1;
  }
  if (x - dispx < (win.window_space.w / 2) - 10) {
    dispx -= 1;
  }
  if (y - dispy < (win.window_space.h / 2) - 10) {
    dispy -= 1;
  }
  if (dispx + win.window_space.w - 1 > width) {
    dispx = width - win.window_space.w;
  }
  if (dispy + win.window_space.h - 1 > height) {
    dispy = height - win.window_space.h;
  }
  if (dispx < 0) {
    dispx = 0;
  }
  if (dispy < 0) {
    dispy = 0;
  }
}

std::vector<std::vector<int>> maze::Grid::GetIntVec() {
  std::vector<std::vector<int>> intvec(width, std::vector<int>(height, 0));
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      intvec[i][j] = grid[i][j];
    }
  }
  return (intvec);
}

void maze::Grid::ErasePos(int x, int y) {
  win.Print(" ", y + 1 - dispy, x + 1 - dispx);
}

void maze::Grid::SetPos(int x, int y) { grid[x][y] = 3; }
