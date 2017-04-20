#include <appareo.h>
#include <pessum.h>
#include "ttt.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace ttt {
  std::vector<std::vector<int>> grid;
}

void ttt::Game() {
  int width = 3, height = 3, win_length = 3;
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
  while (Run(width, height, win_length, ai) == true) {
    fields = NewForm(fields, "New Game", scrwidth / 2, 6);
    width = fields[0].ival;
    height = fields[1].ival;
    win_length = fields[2].ival;
    ai = fields[3].bval;
    if (width * 4 + 1 > scrwidth) {
      width = (scrwidth - 1) / 4;
    }
    if (height * 4 + 1 > scrheight) {
      height = (scrheight - 1) / 4;
    }
  }
}

bool ttt::Run(int width, int height, int win_length, bool ai) {
  int win = windows.size();
  InitializeWindow();
  windows[win].CreateWindow("grid", (width * 4) + 1, (height * 4) + 1, -1,
                            (scrheight - (height * 4 + 1)) / 2, true, false);
  int pl_win = windows.size();
  InitializeWindow();
  if ((width * 4) - 1 < 11) {
    windows[pl_win].CreateWindow("Tic-Tac-Toe", 13, 3, -1,
                                 (scrheight - (height * 4 + 1)) / 2 - 3, true,
                                 true);
  } else {
    windows[pl_win].CreateWindow("Tic-Tac-Toe", (width * 4) + 1, 3, -1,
                                 (scrheight - (height * 4 + 1)) / 2 - 3, true,
                                 true);
  }
  for (int i = 4; i < height * 4; i += 4) {
    wmove(windows[win].windowpointer, i, 0);
    waddch(windows[win].windowpointer, ACS_LTEE);
    whline(windows[win].windowpointer, ACS_HLINE, (width * 4) - 1);
    wmove(windows[win].windowpointer, i, width * 4);
    waddch(windows[win].windowpointer, ACS_RTEE);
  }
  for (int i = 4; i < width * 4; i += 4) {
    wmove(windows[win].windowpointer, 0, i);
    waddch(windows[win].windowpointer, ACS_TTEE);
    wmove(windows[win].windowpointer, 1, i);
    wvline(windows[win].windowpointer, ACS_VLINE, (height * 4) - 1);
    wmove(windows[win].windowpointer, height * 4, i);
    waddch(windows[win].windowpointer, ACS_BTEE);
  }
  for (int i = 4; i < width * 4; i += 4) {
    for (int j = 4; j < height * 4; j += 4) {
      wmove(windows[win].windowpointer, j, i);
      waddch(windows[win].windowpointer, ACS_PLUS);
    }
  }
  windows[win].Update();
  bool running = true;
  int player = 1;
  std::pair<int, int> pos(width / 2, height / 2);
  grid = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
  while (running == true) {
    DrawGrid(width, height, win, pos);
    if (player == 1) {
      windows[pl_win].Clear();
      PrintZ("Player \'X\'", 5, pl_win);
    } else if (player == 2) {
      windows[pl_win].Clear();
      PrintZ("Player \'O\'", 5, pl_win);
    }
    int in = ERR;
    while (in == ERR) {
      in = getch();
    }
    if (in == int('q')) {
      player = 4;
      running = false;
    } else if (in == int(' ')) {
      TerminateWindow(pl_win);
      TerminateWindow(win);
      return (true);
    } else if (in == KEY_UP && pos.second != 0) {
      pos.second--;
    } else if (in == KEY_DOWN && pos.second != height - 1) {
      pos.second++;
    } else if (in == KEY_LEFT && pos.first != 0) {
      pos.first--;
    } else if (in == KEY_RIGHT && pos.first != width - 1) {
      pos.first++;
    } else if (in == 10 && grid[pos.first][pos.second] == 0) {
      grid[pos.first][pos.second] = player;
      if (Win(player, win_length) == false) {
        player++;
        if (player == 3) {
          player = 1;
        }
      } else {
        running = false;
      }
      if (ai == true && player == 2 && CatsGame() == false) {
        pos = AI(win_length);
        grid[pos.first][pos.second] = player;
        if (Win(player, win_length) == false) {
          player++;
          if (player == 3) {
            player = 1;
          }
        } else {
          running = false;
        }
      }
    }
    if (CatsGame()) {
      running = false;
      player = 3;
    }
  }
  windows[pl_win].Clear();
  DrawGrid(width, height, win, pos);
  if (player == 1) {
    PrintZ("X\'s WIN", 5, pl_win);
  } else if (player == 2) {
    PrintZ("O\'s WIN", 5, pl_win);
  } else if (player == 3) {
    PrintZ("Cats Game", 5, pl_win);
  } else if (player == 4) {
    PrintZ("Forfeit", 5, pl_win);
  }
  while (getch() == ERR) {
  }
  TerminateWindow(pl_win);
  TerminateWindow(win);
  return (false);
}

void ttt::DrawGrid(int width, int height, int win, std::pair<int, int> pos) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (i == pos.first && j == pos.second) {
        SetAtt({YELLOW_BACK}, win);
      }
      if (grid[i][j] < 0) {
        SetAtt({GREEN_BACK}, win);
      }
      if (grid[i][j] == 0) {
        Print("   ", (j * 4) + 1, (i * 4) + 1, win);
        Print("   ", (j * 4) + 2, (i * 4) + 1, win);
        Print("   ", (j * 4) + 3, (i * 4) + 1, win);
      } else if (grid[i][j] == 1 || grid[i][j] == -1) {
        Print("\\ /", (j * 4) + 1, (i * 4) + 1, win);
        Print(" X ", (j * 4) + 2, (i * 4) + 1, win);
        Print("/ \\", (j * 4) + 3, (i * 4) + 1, win);
      } else if (grid[i][j] == 2 || grid[i][j] == -2) {
        Print("/-\\", (j * 4) + 1, (i * 4) + 1, win);
        Print("| |", (j * 4) + 2, (i * 4) + 1, win);
        Print("\\-/", (j * 4) + 3, (i * 4) + 1, win);
      }
      if (i == pos.first && j == pos.second) {
        SetAtt({NORMAL}, win);
      }
      if (grid[i][j] < 0) {
        SetAtt({NORMAL}, win);
      }
    }
  }
  windows[win].Update();
}

bool ttt::Win(int player, int length, bool real) {
  bool won = false;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
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

std::pair<int, int> ttt::AI(int length) {
  std::pair<int, int> pos(-1, -1), null(-1, -1), check(0, 0);
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
      pos = std::make_pair(x, y);
      search = false;
    }
  }
  return (pos);
}

std::pair<int, int> ttt::CanWin(int player, int length) {
  std::pair<int, int> pos(-1, -1);
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == 0) {
        grid[i][j] = player;
        if (Win(player, length, false)) {
          grid[i][j] = 0;
          pos = std::make_pair(i, j);
          return (pos);
        }
        grid[i][j] = 0;
      }
    }
  }
  return (pos);
}

bool ttt::CatsGame() {
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] == 0) {
        return (false);
      }
    }
  }
  return (true);
}
