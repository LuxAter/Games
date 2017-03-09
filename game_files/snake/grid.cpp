#include <appareo.h>
#include <vector>
#include "grid.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

void snake::Grid::Init(int sizex, int sizey) {
  grid.clear();
  snakegrid.clear();
  totalgrid.clear();
  std::vector<int> col(sizey, 0);
  for (int i = 0; i < sizex; i++) {
    grid.push_back(col);
  }
  snakegrid = grid;
  totalgrid = grid;
  lasttotal = grid;
  width = sizex;
  height = sizey;
  obsticalcount = 0;
  applecount = 0;
  currentapples = 0;
  win = windows.size();
  InitializeWindow();
  windows[win].CreateWindow("Snake", sizex + 2, sizey + 2, -1,
                            (scrheight - sizey + 2) / 2, true, true);

  scorewin = windows.size();
  InitializeWindow();
  windows[scorewin].CreateWindow("Score", sizex + 2, 3, -1,
                                 (scrheight - sizey + 2) / 2 - 3, true, true);
}

void snake::Grid::GenObsticals(int count) {
  while (count > 0) {
    int x = rand() % width;
    int y = rand() % height;
    if (Empty(x, y) == true) {
      grid[x][y] = -1;
      count--;
    }
  }
}

void snake::Grid::SetAppleCount(int count) { applecount = count; }

void snake::Grid::GenApple() {
  while (currentapples < applecount) {
    int x = rand() % width;
    int y = rand() % height;
    if (Empty(x, y) == true) {
      grid[x][y] = -2;
      currentapples++;
      applepos.push_back(std::pair<int, int>(x, y));
    }
  }
}

void snake::Grid::SumGrids() {
  totalgrid = grid;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (snakegrid[i][j] != 0) {
        totalgrid[i][j] = snakegrid[i][j];
      }
    }
  }
}

void snake::Grid::Show() {
  GenApple();
  SumGrids();
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (lasttotal[i][j] != totalgrid[i][j]) {
        lasttotal[i][j] = totalgrid[i][j];
        std::string str = " ";
        if (totalgrid[i][j] == -1) {
          str = "#";
        } else if (totalgrid[i][j] == -2) {
          SetAtt({GREEN_TEXT}, win);
          str = "*";
        } else if (totalgrid[i][j] == 1) {
          SetAtt({WHITE_BACK}, win);
        } else if (totalgrid[i][j] == 2) {
          SetAtt({BLUE_BACK}, win);
        } else if (totalgrid[i][j] == 3) {
          SetAtt({RED_BACK}, win);
        } else if (totalgrid[i][j] == 4) {
          SetAtt({YELLOW_BACK}, win);
        } else if (totalgrid[i][j] == 5) {
          SetAtt({GREEN_BACK}, win);
        }
        Print(str, j + 1, i + 1, win, false);
        if (totalgrid[i][j] == -2 || totalgrid[i][j] > 0) {
          SetAtt({NORMAL}, win);
        }
      }
    }
  }
  windows[win].Update();
}

bool snake::Grid::Empty(int x, int y) {
  if (x < width && x >= 0 && y < height && y >= 0) {
    if (grid[x][y] == 0 && snakegrid[x][y] == 0) {
      return (true);
    } else {
      return (false);
    }
  }
  return (false);
}

bool snake::Grid::Safe(int x, int y) {
  if (x < width && x >= 0 && y < height && y >= 0) {
    if ((grid[x][y] == 0 || grid[x][y] == -2) && (snakegrid[x][y] == 0)) {
      return (true);
    }
  }
  return (false);
}

bool snake::Grid::Apple(int x, int y) {
  if (x < width && x >= 0 && y < height && y >= 0) {
    if (grid[x][y] == -2) {
      return (true);
    }
  }
  return (false);
}

void snake::Grid::Delete() {
  TerminateWindow(scorewin);
  TerminateWindow(win);
  grid.clear();
  snakegrid.clear();
  totalgrid.clear();
}

void snake::Grid::SetScoreCount(int count) { scorecount = count; }

void snake::Grid::ESnake(int x, int y) { snakegrid[x][y] = 0; }
void snake::Grid::SSnake(int x, int y, int val) { snakegrid[x][y] = val; }
void snake::Grid::SObs(int x, int y) { grid[x][y] = -1; }
void snake::Grid::Eat(int x, int y) {
  if (grid[x][y] == -2) {
    for (int i = 0; i < applepos.size(); i++) {
      if (applepos[i].first == x && applepos[i].second == y) {
        applepos.erase(applepos.begin() + i);
        break;
      }
    }
    grid[x][y] = 0;
    currentapples--;
    GenApple();
  }
}

void snake::Grid::DisplayScore(int score, int index) {
  if (index == 1) {
    SetAtt({WHITE_TEXT}, scorewin);
  } else if (index == 2) {
    SetAtt({BLUE_TEXT}, scorewin);
  } else if (index == 3) {
    SetAtt({RED_TEXT}, scorewin);
  } else if (index == 4) {
    SetAtt({YELLOW_TEXT}, scorewin);
  } else if (index == 5) {
    SetAtt({GREEN_TEXT}, scorewin);
  }
  int pos = (index * (width / scorecount)) - (width / scorecount);
  Print(std::to_string(score), 1, pos + 1, scorewin);
}
