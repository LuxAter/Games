#include <ostendo.h>
#include <pessum.h>
#include "fiar.hpp"

using namespace ostendo;

namespace fiar {
  std::vector<std::vector<int>> grid;
}

void fiar::Game() {
  int width = 6, height = 6, win_length = 4;
  bool ai = true;
  std::vector<Var> fields = {Var("Width", width), Var("Height", height), Var("Length", win_length), Var("AI", ai)};
  while (Run(width, height + 1, win_length, ai) == true) {
    Window win(0.5, 0.5, 0.25, 0.25);
    win.ToggleBorder();
    win.ToggleTitle("Four In A Row");
    fields = Form(win, fields);
    win.DelWin();
    width = fields[0].Int();
    height = fields[1].Int();
    win_length = fields[2].Int();
    ai = fields[3].Bool();
    if (width * 2 + 1 > std_scr.w) {
      width = (std_scr.w - 1) / 2;
    }
    if ((height + 1) * 2 + 1 > std_scr.h) {
      height = (std_scr.h - 1) / 2;
    }
  }
}

bool fiar::Run(int width, int height, int win_length, bool ai) {
  Window win((width * 2) + 1, (height * 2) + 1, (std_scr.w - (width * 2 + 1)) / 2, (std_scr.h - (height * 2 + 1)) / 2);
  Window pl_win;
  if((width * 2) - 1 < 15){
    pl_win = Window(15, 3, (std_scr.w - 15) / 2, (std_scr.h - (height * 2 + 1)) / 2 - 3);
  }else{
    pl_win = Window((width * 2) + 1, 3, (std_scr.w - (width * 2 + 1)) / 2, (std_scr.h - (height * 2 + 1)) / 2 - 3);
  }
  pl_win.ToggleBorder();
  pl_win.ToggleTitle("Four in A Row");
  
  win.ToggleBorder();
  for (int i = 2; i < height * 2; i += 2) {
    wmove(win(), i, 0);
    waddch(win(), ACS_LTEE);
    whline(win(), ACS_HLINE, (width * 2) - 1);
    wmove(win(), i, width * 2);
    waddch(win(), ACS_RTEE);
  }
  for (int i = 2; i < width * 2; i += 2) {
    wmove(win(), 1, i);
    wvline(win(), ACS_VLINE, (height * 2) - 1);
    wmove(win(), height * 2, i);
    waddch(win(), ACS_BTEE);
  }
  for (int i = 2; i < width * 2; i += 2) {
    for (int j = 2; j < height * 2; j += 2) {
      wmove(win(), j, i);
      waddch(win(), ACS_PLUS);
    }
  }
  //win.ToggleBorder();
  win.Update();
  int pos = width / 2, player = 1;
  grid = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
  bool running = true, update = true;
  while (running == true) {
    if (update == true) {
      update = false;
      DrawGrid(width, height, win, pos);
      pl_win.Clear();
      pl_win.Print("#cPlayer %i", player);
    }
    int in = getch();
    if (in == int('q')) {
      player = 4;
      running = false;
    } else if (in == int(' ')) {
      pl_win.DelWin();
      win.DelWin();
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
  pl_win.Clear();
  if (player == 1 || player == 2) {
    pl_win.Print("#cPlayer %i Wins", player);
  } else if (player == 3) {
    pl_win.Print("#cDraw");
  }
  DrawGrid(width, height, win, -1);
  while (player != 4 && getch() == ERR) {
  }
  pl_win.DelWin();
  win.DelWin();
  return (false);
}

void fiar::DrawGrid(int width, int height, Window win, int pos) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (i == pos && j == 0) {
        win.ColorOn(4);
      }
      if (grid[i][j] < 0) {
        win.ColorOn(2);
      } else if (grid[i][j] == 1) {
        win.ColorOn(3);
      } else if (grid[i][j] == 2) {
        win.ColorOn(1);
      }
      win.SetCurs(j * 2 + 1, i * 2 + 1);
      win.Print(" ");
      if(i == pos && j == 0){
        win.ColorOff(4);
      }
      if(grid[i][j] < 0){
        win.ColorOff(2);
      } else if(grid[i][j] == 1){
        win.ColorOff(3);
      } else if(grid[i][j] == 2){
        win.ColorOff(1);
      }
    }
  }
  win.Update();
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
