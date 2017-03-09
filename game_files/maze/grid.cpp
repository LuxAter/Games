#include <appareo.h>
#include <pessum.h>
#include <vector>
#include "grid.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

void maze::Grid::Init(int sizex, int sizey, bool winc) {
  dispx = 0;
  dispy = 0;
  lastdispx = -1;
  lastdispy = -1;
  grid.clear();
  score = 0;
  wincheck = winc;
  if (winc == true) {
    win = windows.size();
    InitializeWindow();
    if (sizex >= scrwidth - 2 && sizey >= scrheight - 2) {
      windows[win].CreateWindow("Maze", scrwidth, scrheight - 3, -1, 3);
    } else if (sizex >= scrwidth - 2 && sizey < scrheight - 2) {
      windows[win].CreateWindow("Maze", scrwidth, sizey + 2, -1,
                                (scrheight - sizey + 2) / 2);
    } else if (sizex < scrwidth - 2 && sizey >= scrheight - 2) {
      windows[win].CreateWindow("Maze", sizex + 2, scrheight - 3, -1, 3);
    } else {
      windows[win].CreateWindow("Maze", sizex + 2, sizey + 2, -1,
                                (scrheight - sizey + 2) / 2);
    }
    countwin = windows.size();
    InitializeWindow();
    windows[countwin].CreateWindow("Move Count", scrwidth, 3, -1, 0, true,
                                   true);

    std::vector<std::vector<int>> dispgrid(
        windows[win].width - 2, std::vector<int>(windows[win].height - 2, -5));
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
        windows[win].width - 2, std::vector<int>(windows[win].height - 2, 0));
    for (int i = dispx; i < dispx + windows[win].width - 2; i++) {
      for (int j = dispy; j < dispy + windows[win].height - 2; j++) {
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
            SetAtt({WHITE_BACK}, win);
          }
          if (dispgrid[i][j] == 1) {
            SetAtt({NORMAL}, win);
          }
          if (dispgrid[i][j] == 2) {
            SetAtt({GREEN_BACK}, win);
          }
          if (dispgrid[i][j] == 3) {
            SetAtt({BLUE_BACK}, win);
          }
          Print(str, j + 1, i + 1, win, false);
        }
      }
    }
    SetAtt({NORMAL}, win);
    windows[win].Update();
    lastdispx = dispx;
    lastdispy = dispy;
  }
  windows[countwin].Clear();
  PrintZ(std::to_string(score), 5, countwin);
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
    TerminateWindow(countwin);
    TerminateWindow(win);
  }
}

void maze::Grid::ShowPos(int x, int y) {
  SetAtt({BLUE_BACK}, win);
  Print(" ", y + 1 - dispy, x + 1 - dispx, win);
  SetAtt({NORMAL}, win);
}

void maze::Grid::SetDisp(int x, int y) {
  if (x - dispx > (windows[win].width / 2) + 10) {
    dispx += 1;
  }
  if (y - dispy > (windows[win].height / 2) + 10) {
    dispy += 1;
  }
  if (x - dispx < (windows[win].width / 2) - 10) {
    dispx -= 1;
  }
  if (y - dispy < (windows[win].height / 2) - 10) {
    dispy -= 1;
  }
  if (dispx + windows[win].width - 1 > width) {
    dispx = width - windows[win].width + 2;
  }
  if (dispy + windows[win].height - 1 > height) {
    dispy = height - windows[win].height + 2;
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
  SetAtt({NORMAL}, win);
  Print(" ", y + 1 - dispy, x + 1 - dispx, win, false);
}

void maze::Grid::SetPos(int x, int y) { grid[x][y] = 3; }
