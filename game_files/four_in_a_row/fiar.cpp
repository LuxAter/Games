#include <appareo.h>
#include <pessum.h>
#include "fiar.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace fiar {
  std::vector<std::vector<int>> grid;
}

void fiar::Game() {
  int width = 6, height = 6, win_length = 4;
  bool ai = true;
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
  new_field.name = "Length";
  new_field.type = 1;
  new_field.sval = std::to_string(win_length);
  fields.push_back(new_field);
  new_field.name = "AI";
  new_field.type = 3;
  new_field.bval = ai;
  fields.push_back(new_field);
  while (Run(width, height + 1, win_length, ai) == true) {
    fields = NewForm(fields, "New Game", scrwidth / 2, 6);
    width = fields[0].ival;
    height = fields[1].ival;
    win_length = fields[2].ival;
    ai = fields[3].bval;
    if (width * 2 + 1 > scrwidth) {
      width = (scrwidth - 1) / 2;
    }
    if ((height + 1) * 2 + 1 > scrheight) {
      height = (scrheight - 1) / 2;
    }
  }
}

bool fiar::Run(int width, int height, int win_length, bool ai) {
  int win = windows.size();
  InitializeWindow();
  windows[win].CreateWindow("grid", (width * 2) + 1, (height * 2) + 1, -1,
                            (scrheight - (height * 2 + 1)) / 2, true, false);
  int pl_win = windows.size();
  InitializeWindow();
  if ((width * 2) - 1 < 15) {
    windows[pl_win].CreateWindow("Four In A Row", 15, 3, -1,
                                 (scrheight - (height * 2 + 1)) / 2 - 3, true,
                                 true);
  } else {
    windows[pl_win].CreateWindow("Four In A Row", (width * 2) + 1, 3, -1,
                                 (scrheight - (height * 2 + 1)) / 2 - 3, true,
                                 true);
  }
  for (int i = 2; i < height * 2; i += 2) {
    wmove(windows[win].windowpointer, i, 0);
    waddch(windows[win].windowpointer, ACS_LTEE);
    whline(windows[win].windowpointer, ACS_HLINE, (width * 2) - 1);
    wmove(windows[win].windowpointer, i, width * 2);
    waddch(windows[win].windowpointer, ACS_RTEE);
  }
  for (int i = 2; i < width * 2; i += 2) {
    wmove(windows[win].windowpointer, 1, i);
    wvline(windows[win].windowpointer, ACS_VLINE, (height * 2) - 1);
    wmove(windows[win].windowpointer, height * 2, i);
    waddch(windows[win].windowpointer, ACS_BTEE);
  }
  for (int i = 2; i < width * 2; i += 2) {
    for (int j = 2; j < height * 2; j += 2) {
      wmove(windows[win].windowpointer, j, i);
      waddch(windows[win].windowpointer, ACS_PLUS);
    }
  }
  windows[win].Update();
  int pos = width / 2, player = 1;
  grid = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
  bool running = true, update = true;
  while (running == true) {
    if (update == true) {
      update = false;
      DrawGrid(width, height, win, pos);
      if (player == 1) {
        windows[pl_win].Clear();
        PrintZ("Player 1", 5, pl_win);
      } else if (player == 2) {
        windows[pl_win].Clear();
        PrintZ("Player 2", 5, pl_win);
      }
    }
    int in = getch();
    if (in == int('q')) {
      player = 4;
      running = false;
    } else if (in == int(' ')) {
      TerminateWindow(pl_win);
      TerminateWindow(win);
      return (true);
    } else if (in == KEY_RIGHT && pos < width - 1) {
      pos++;
      update = true;
    } else if (in == KEY_LEFT && pos > 0) {
      pos--;
      update = true;
    } else if (in == 10 && grid[pos][1] == 0) {
      for (int i = 1; i < height && grid[pos][i] == 0; i++) {
        grid[pos][i] = player;
        DrawGrid(width, height, win, pos);
        Pause(10);
        if (i != height - 1 && grid[pos][i + 1] == 0) {
          grid[pos][i] = 0;
        }
      }
      if (Win(player, win_length, true) == false) {
        player++;
        if (player == 3) {
          player = 1;
        }
      } else {
        running = false;
      }
      if (ai == true && player == 2 && CatsGame() == false) {
        pos = AI(win_length);
        for (int i = 1; i < height && grid[pos][i] == 0; i++) {
          grid[pos][i] = player;
          DrawGrid(width, height, win, pos);
          Pause(10);
          if (i != height - 1 && grid[pos][i + 1] == 0) {
            grid[pos][i] = 0;
          }
        }
        if (Win(player, win_length) == false) {
          player++;
          if (player == 3) {
            player = 1;
          }
        } else {
          running = false;
        }
      }
      update = true;
    }
    if (CatsGame() == true) {
      player = 3;
      running = false;
    }
  }
  windows[pl_win].Clear();
  if (player == 1) {
    PrintZ("Player 1 Wins", 5, pl_win);
  } else if (player == 2) {
    PrintZ("Player 2 Wins", 5, pl_win);
  } else if (player == 3) {
    PrintZ("Draw", 5, pl_win);
  }
  DrawGrid(width, height, win, -1);
  while (player != 4 && getch() == ERR) {
  }
  TerminateWindow(pl_win);
  TerminateWindow(win);
  return (false);
}

void fiar::DrawGrid(int width, int height, int win, int pos) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (i == pos && j == 0) {
        SetAtt({BLUE_BACK}, win);
      }
      if (grid[i][j] < 0) {
        SetAtt({GREEN_BACK}, win);
      } else if (grid[i][j] == 1) {
        SetAtt({YELLOW_BACK}, win);
      } else if (grid[i][j] == 2) {
        SetAtt({RED_BACK}, win);
      }
      Print(" ", (j * 2) + 1, (i * 2) + 1, win);
      SetAtt({NORMAL}, win);
    }
  }
  windows[win].Update();
}

void fiar::Pause(int tics) {
  for (int i = 0; i < tics; i++) {
    getch();
  }
}

bool fiar::Win(int player, int length, bool real) {
  bool won = false;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 1; j < grid[i].size(); j++) {
      int streakh = length, streakv = length, streakd = length,
          streaknd = length;
      for (int k = 0; k < length; k++) {
        if (i + k < grid.size() && grid[i + k][j] == player) {
          streakh--;
        } else {
          streakh = length;
        }
        if (j + k < grid[i].size() && grid[i][j + k] == player) {
          streakv--;
        } else {
          streakv = length;
        }
        if (i + k < grid.size() && j + k < grid[i + k].size() &&
            grid[i + k][j + k] == player) {
          streakd--;
        } else {
          streakd = length;
        }
        if (i - k < grid.size() && j + k < grid[i - k].size() &&
            grid[i - k][j + k] == player) {
          streaknd--;
        } else {
          streaknd = length;
        }
      }
      if (streakh == 0 || streakv == 0 || streakd == 0 || streaknd == 0) {
        won = true;
      }
      if (real == true) {
        if (streakh == 0) {
          for (int k = 0; k < length; k++) {
            if (grid[i + k][j] > 0) {
              grid[i + k][j] *= -1;
            }
          }
        }
        if (streakv == 0) {
          for (int k = 0; k < length; k++) {
            if (grid[i][j + k] > 0) {
              grid[i][j + k] *= -1;
            }
          }
        }
        if (streakd == 0) {
          for (int k = 0; k < length; k++) {
            if (grid[i + k][j + k] > 0) {
              grid[i + k][j + k] *= -1;
            }
          }
        }
        if (streaknd == 0) {
          for (int k = 0; k < length; k++) {
            if (grid[i - k][j + k] > 0) {
              grid[i - k][j + k] *= -1;
            }
          }
        }
      }
    }
  }
  return (won);
}

bool fiar::CatsGame() {
  bool open = false;
  for (int i = 0; i < grid.size() && open == false; i++) {
    for (int j = 1; j < grid[i].size() && open == false; j++) {
      if (grid[i][j] == 0) {
        open = true;
      }
    }
  }
  return (!open);
}

int fiar::AI(int length) {
  int pos = -1, null = -1, check = 0;
  check = CanWin(2, length);
  if (check != null) {
    pos = check;
  }
  check = CanWin(1, length);
  if (check != null && pos == null) {
    pos = check;
  }
  bool search = true;
  while (search == true && pos == null) {
    int x = rand() % grid.size();
    int y = rand() % grid[x].size();
    if (grid[x][y] == 0) {
      pos = x;
      search = false;
    }
  }
  return (pos);
}

int fiar::CanWin(int player, int length) {
  int pos = -1;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 1; j < grid[i].size(); j++) {
      if (grid[i][j] == 0) {
        grid[i][j] = player;
        if (Win(player, length, false)) {
          grid[i][j] = 0;
          pos = i;
          return (pos);
        }
        grid[i][j] = 0;
      }
    }
  }
  return (pos);
}
