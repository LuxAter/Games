#include <appareo.h>
#include <pessum.h>
#include <vector>
#include "grid.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

void minesw::Grid::Init(int sizex, int sizey, int mine) {
  rawgrid.clear();
  displaygrid.clear();
  width = sizex;
  height = sizey;
  minecount = mine;
  win = windows.size();
  InitializeWindow();
  windows[win].CreateWindow("Mine", sizex + 2, sizey + 2, -1,
                            (scrheight - sizey + 2) / 2);

  countwin = windows.size();
  InitializeWindow();
  windows[countwin].CreateWindow("Mine Sweeper", sizex + 2, 3, -1,
                                 (scrheight - sizey + 2) / 2 - 3, true, true);
  GenGrid();
}

bool minesw::Grid::CheckPos(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    if (rawgrid[x][y] == true) {
      return (true);
    }
  }
  return (false);
}

void minesw::Grid::ClearSpace(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y < height &&
      (displaygrid[x][y] == -1 || displaygrid[x][y] == -3)) {
    if (displaygrid[x][y] == -3) {
      Flag(x, y);
    }
    displaygrid[x][y] = GetCount(x, y);
    if (displaygrid[x][y] == 0) {
      for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {
          ClearSpace(i, j);
        }
      }
    }
  }
}

void minesw::Grid::Show(int x, int y) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      std::string str = std::to_string(displaygrid[i][j]);
      std::vector<int> atts;
      if (i == x && j == y) {
        atts.push_back(YELLOW_BACK);
      }
      if (displaygrid[i][j] == -1) {
        str = "#";
      } else if (displaygrid[i][j] == -2) {
        atts = {RED_TEXT, BLACK_BACK};
        str = "X";
      } else if (displaygrid[i][j] == -3) {
        str = "!";
        atts.push_back(RED_TEXT);
      } else if (displaygrid[i][j] == 0) {
        str = " ";
      } else if (displaygrid[i][j] == 1) {
        atts.push_back(BLUE_TEXT);
      } else if (displaygrid[i][j] == 2) {
        atts.push_back(GREEN_TEXT);
      } else if (displaygrid[i][j] == 3) {
        atts.push_back(RED_TEXT);
      } else if (displaygrid[i][j] == 4) {
        atts.push_back(BLUE_TEXT);
        atts.push_back(DIM);
      } else if (displaygrid[i][j] == 5) {
        atts.push_back(RED_TEXT);
        atts.push_back(DIM);
      } else if (displaygrid[i][j] == 6) {
        atts.push_back(CYAN_TEXT);
      } else if (displaygrid[i][j] == 7) {
        atts.push_back(CYAN_TEXT);
        atts.push_back(DIM);
      } else if (displaygrid[i][j] == 8) {
        atts.push_back(MAGENTA_TEXT);
      }
      SetAtt(atts, win);
      Print(str, j + 1, i + 1, win, false);
      SetAtt({NORMAL}, win);
    }
  }
  windows[win].Update();
  windows[countwin].Clear();
  PrintZ(std::to_string(minecount - flagcount), 4, countwin);
  PrintZ(std::to_string(score), 6, countwin);
}

void minesw::Grid::GenGrid() {
  std::vector<std::vector<bool>> grid(width, std::vector<bool>(height, false));
  std::vector<std::vector<int>> dispgrid(width, std::vector<int>(height, -1));
  rawgrid = grid;
  displaygrid = dispgrid;
  lastgrid = displaygrid;
}

void minesw::Grid::GenMines(int xpos, int ypos) {
  int count = 0;
  while (count < minecount) {
    int x = rand() % width;
    int y = rand() % height;
    if (displaygrid[x][y] == -1 && rawgrid[x][y] == false &&
        (x > xpos + 1 || x < xpos - 1) && (y > ypos + 1 || y < ypos - 1)) {
      rawgrid[x][y] = true;
      count++;
    }
  }
}

void minesw::Grid::Delete() {
  rawgrid.clear();
  displaygrid.clear();
  TerminateWindow(countwin);
  TerminateWindow(win);
}

int minesw::Grid::GetCount(int x, int y) {
  int count = 0;
  for (int i = x - 1; i < x + 2; i++) {
    for (int j = y - 1; j < y + 2; j++) {
      if (CheckPos(i, j) == true) {
        count++;
      }
    }
  }
  return (count);
}

void minesw::Grid::Flag(int x, int y) {
  if (displaygrid[x][y] == -1) {
    displaygrid[x][y] = -3;
    flagcount++;
  } else if (displaygrid[x][y] == -3) {
    displaygrid[x][y] = -1;
    flagcount--;
  }
}

bool minesw::Grid::Compleated() {
  bool solved = true;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (displaygrid[i][j] < 0 && rawgrid[i][j] == false) {
        solved = false;
        return (solved);
      }
    }
  }
  return (solved);
}
