#include <appareo.h>
#include <algorithm>
#include <ctime>
#include <vector>
#include "grid.hpp"
#include "minesw.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace minesw {
  Grid grid;
}

void minesw::Game() {
  int width = 15;
  int height = 15;
  int mines = 27;
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
  newfield.name = "Mine Count";
  newfield.type = 1;

  fields.push_back(newfield);
  while (Run(width, height, mines) == true) {
    grid.Delete();
    fields = NewForm(fields, "New Game", scrwidth / 2, 8);
    width = fields[0].ival;
    height = fields[1].ival;
    mines = fields[2].ival;
    if (width > scrwidth - 2) {
      width = scrwidth - 2;
    }
    if (height > scrheight - 6) {
      height = scrheight - 6;
    }
    if (mines > (width * height) * 0.95) {
      mines = (width * height) * 0.95;
    }
    if (width < 10) {
      width = 10;
    }
    if (height < 10) {
      height = 10;
    }
    if (mines < 1) {
      mines = 1;
    }
  }
  grid.Delete();
}

bool minesw::Run(int width, int height, int mines) {
  int currentx = width / 2, currenty = height / 2;
  grid.Init(width, height, mines);
  bool running = true;
  bool first = true;
  grid.Show(currentx, currenty);
  int in = ERR;
  int tic_count = 0;
  while (running == true) {
    grid.Show(currentx, currenty);
    in = getch();
    tic_count++;
    if (tic_count >= 500) {
      grid.score++;
      tic_count = 0;
    }
    if (in == KEY_UP && currenty > 0) {
      currenty--;
    }

    if (in == KEY_DOWN && currenty < height - 1) {
      currenty++;
    }

    if (in == KEY_LEFT && currentx > 0) {
      currentx--;
    }

    if (in == KEY_RIGHT && currentx < width - 1) {
      currentx++;
    }
    if (in == 10) {
      if (first == true) {
        grid.GenMines(currentx, currenty);
        first = false;
      }
      if (grid.CheckPos(currentx, currenty) == false) {
        grid.ClearSpace(currentx, currenty);
      } else {
        running = false;
      }
    }
    if (in == int('q')) {
      return (false);
    }
    if (in == int('f')) {
      grid.Flag(currentx, currenty);
    }
    if (in == int(' ')) {
      return (true);
    }
    if (grid.Compleated() == true) {
      Score();
      running = false;
    }
  }
  return (true);
}

void minesw::Score() {
  int scorewindow = windows.size();
  InitializeWindow();
  windows[scorewindow].CreateWindow("SCORE", scrwidth / 5, 3, -1, -1, true,
                                    true);
  PrintZ(std::to_string(grid.score), 5, scorewindow);
  while (getch() == ERR) {
  }
  TerminateWindow(scorewindow);
}
