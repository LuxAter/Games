#include <appareo.h>
#include <pessum.h>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <vector>
#include "grid.hpp"
#include "maze.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace maze {
  Grid grid;
}

void maze::Game() {
  int width = scrwidth - 3;
  int height = scrheight - 5;
  if (width % 2 == 0) {
    width++;
  }
  if (height % 2 == 0) {
    height++;
  }
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
  newfield.name = "Analize";
  newfield.type = 3;
  newfield.bval = false;
  fields.push_back(newfield);
  bool term = false;
  while (Run(width, height) == true) {
    grid.Delete();
    fields = NewForm(fields, "New Game", scrwidth / 2, 8);
    width = fields[0].ival;
    height = fields[1].ival;
    if (width % 2 == 0) {
      width++;
    }
    if (height % 2 == 0) {
      height++;
    }
    if (fields[2].bval == true) {
      break;
    }
  }
  if (fields[2].bval == true) {
    term = true;
    fields.clear();
    newfield.name = "Count";
    newfield.type = 1;
    newfield.sval = "100";
    fields.push_back(newfield);
    newfield.name = "Meta";
    newfield.type = 3;
    newfield.bval = false;
    fields.push_back(newfield);
    newfield.name = "Save";
    newfield.type = 3;
    newfield.bval = true;
    fields.push_back(newfield);
    fields = NewForm(fields, "Analize Settings", scrwidth / 2, 8);
    std::ofstream out("Output.txt");
    long double total = 0;
    bool meta = fields[1].bval;
    if (fields[1].bval == false) {
      CreateProgressBar("Solving Mazes");
      for (int i = 0; i < fields[0].ival; i++) {
        UpdateProgressBar((double)i / (double)fields[0].ival);
        grid.Init(width, height, false);
        AutoRun(true);
        total += grid.score;
        if (fields[2].bval == true) {
          out << grid.score << ",";
        }
        grid.Delete();
      }
      total /= fields[0].ival;
      out << ":\n" << total;
      TerminateProgressBar();
    } else if (fields[1].bval == true) {
      int count = fields[0].ival;
      fields.clear();
      newfield.name = "Min Size";
      newfield.type = 1;
      newfield.sval = "10";
      fields.push_back(newfield);
      newfield.name = "Max Size";
      newfield.type = 1;
      newfield.sval = "100";
      fields.push_back(newfield);
      fields = NewForm(fields, "Meta Settings", scrwidth / 2, 8);
      if (fields[0].ival % 2 == 0) {
        fields[0].ival++;
      }
      if (fields[1].ival % 2 == 0) {
        fields[1].ival++;
      }
      int totalmazes = (fields[1].ival - fields[0].ival) / 2;
      totalmazes *= count;
      int index = 1;
      CreateProgressBar("Solving Mazes");
      for (int i = fields[0].ival; i <= fields[1].ival; i += 2) {
        total = 0;
        out << i << "x" << i;
        for (int j = 0; j < count; j++) {
          UpdateProgressBar((double)index / (double)totalmazes);
          grid.Init(i, i, false);
          AutoRun(true);
          total += grid.score;
          grid.Delete();
          index++;
        }
        total /= (double)count;
        out << ":" << total << "\n";
      }
      TerminateProgressBar();
    }
    out.close();
    if (meta == false) {
      int scorewindow = windows.size();
      InitializeWindow();
      windows[scorewindow].CreateWindow("Average Moves", scrwidth / 5, 3, -1,
                                        -1, true, true);
      PrintZ(std::to_string(total), 5, scorewindow);
      while (getch() == ERR) {
      }
      TerminateWindow(scorewindow);
    }
  }
  if (term == false) {
    grid.Delete();
  }
}

bool maze::Run(int width, int height) {
  int currentx = 1, currenty = 1;
  grid.Init(width, height);
  bool running = true;
  int in = ERR, last = ERR;
  while (running == true) {
    grid.SetDisp(currentx, currenty);
    grid.Show();
    grid.ShowPos(currentx, currenty);
    grid.ErasePos(currentx, currenty);
    in = getch();
    if (in == ERR) {
      in = last;
    }
    last = in;
    if (in == KEY_UP && grid.CheckPos(currentx, currenty - 1)) {
      currenty--;
      grid.score++;
    }

    if (in == KEY_DOWN && grid.CheckPos(currentx, currenty + 1)) {
      currenty++;
      grid.score++;
    }

    if (in == KEY_LEFT && grid.CheckPos(currentx - 1, currenty)) {
      currentx--;
      grid.score++;
    }

    if (in == KEY_RIGHT && grid.CheckPos(currentx + 1, currenty)) {
      currentx++;
      grid.score++;
    }
    if (in == int('q')) {
      return (false);
    }
    if (in == int(' ')) {
      return (true);
    }
    if (in == int('s')) {
      AutoRun(false);
      Score();
      return (true);
    }
    if (currentx == width - 2 && currenty == height - 2) {
      Score();
      running = false;
    }
  }
  return (true);
}

void maze::AutoRun(bool fast) {
  std::vector<std::vector<int>> aimaze = grid.GetIntVec();
  std::pair<int, int> pos(1, 1);
  std::pair<int, int> end(grid.width - 2, grid.height - 2);
  int delaytick = CLOCKS_PER_SEC / 400;
  clock_t currenttime = clock();
  bool solved = false;
  int dir = 2, index = 0;
  while (solved == false) {
    index++;
    bool up = grid.CheckPos(pos.first, pos.second - 1);
    bool down = grid.CheckPos(pos.first, pos.second + 1);
    bool left = grid.CheckPos(pos.first - 1, pos.second);
    bool right = grid.CheckPos(pos.first + 1, pos.second);
    aimaze[pos.first][pos.second] = index;
    if (dir == 4) {
      if (down == true) {
        pos.second++;
        dir = 2;
      } else if (right == true) {
        pos.first++;
      } else if (up == true) {
        pos.second--;
        dir = 1;

      } else if (left == true) {
        pos.first--;
        dir = 3;
      }
    } else if (dir == 1) {
      if (right == true) {
        pos.first++;
        dir = 4;
      } else if (up == true) {
        pos.second--;

      } else if (left == true) {
        pos.first--;
        dir = 3;

      } else if (down == true) {
        pos.second++;
        dir = 2;
      }
    } else if (dir == 3) {
      if (up == true) {
        pos.second--;
        dir = 1;
      } else if (left == true) {
        pos.first--;

      } else if (down == true) {
        pos.second++;
        dir = 2;

      } else if (right == true) {
        pos.first++;
        dir = 4;
      }
    } else if (dir == 2) {
      if (left == true) {
        pos.first--;
        dir = 3;
      } else if (down == true) {
        pos.second++;

      } else if (right == true) {
        pos.first++;
        dir = 4;

      } else if (up == true) {
        pos.second--;
        dir = 1;
      }
    }
    if (pos.first == end.first && pos.second == end.second) {
      aimaze[pos.first][pos.second] = index + 1;
      solved = true;
    }
  }
  solved = false;
  pos.first = 1;
  pos.second = 1;
  while (solved == false) {
    int bestdir = 4;
    int best = aimaze[pos.first + 1][pos.second];
    if (aimaze[pos.first][pos.second - 1] > best) {
      bestdir = 1;
      best = aimaze[pos.first][pos.second - 1];
    }
    if (aimaze[pos.first - 1][pos.second] > best) {
      bestdir = 3;
      best = aimaze[pos.first - 1][pos.second];
    }
    if (aimaze[pos.first][pos.second + 1] > best) {
      bestdir = 2;
      best = aimaze[pos.first][pos.second + 1];
    }
    if (bestdir == 4) {
      pos.first++;
    } else if (bestdir == 1) {
      pos.second--;
    } else if (bestdir == 3) {
      pos.first--;
    } else if (bestdir == 2) {
      pos.second++;
    }
    grid.score++;
    if (fast == false) {
      grid.SetDisp(pos.first, pos.second);
      grid.Show();
      grid.ShowPos(pos.first, pos.second);
      grid.SetPos(pos.first, pos.second);
      while (clock() < currenttime + delaytick) {
      }
      currenttime = clock();
    }
    if (pos.first == end.first && pos.second == end.second) {
      solved = true;
    }
  }
}

void maze::Score() {
  int scorewindow = windows.size();
  InitializeWindow();
  windows[scorewindow].CreateWindow("SCORE", scrwidth / 5, 3, -1, -1, true,
                                    true);
  PrintZ(std::to_string(grid.score), 5, scorewindow);
  while (getch() == ERR) {
  }
  TerminateWindow(scorewindow);
}
