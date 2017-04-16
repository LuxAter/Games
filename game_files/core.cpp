#include <ostendo.h>
#include <map>
#include <string>
#include <vector>
#include "core.hpp"
#include "game_headers.hpp"

using namespace ostendo;

void game::MainMenu() {
  std::map<std::string, void (*)()> games;
  games["FOUR IN A ROW"] = fiar::Game;
  /*games["MAZE"] = maze::Game;
  games["MINE SWEEPER"] = minesw::Game;
  games["PACMAN"] = pacman::Game;
  games["SNAKE"] = snake::Game;
  games["TETRIS"] = tetris::Game;
  games["TIC TAC TOE"] = ttt::Game;
  
  */std::vector<std::string> options = {"Game A", "Game b", "Game C"};
  for (std::map<std::string, void (*)()>::iterator it = games.begin();
       it != games.end(); ++it) {
    options.push_back(it->first);
  }
  options.push_back("QUIT");
  std::string input = "";
  ostendo::Window win(0.5, 0.5, 0.25, 0.25);
  win.ToggleBorder();
  win.ToggleTitle("Games");
  while (input != "QUIT") {
    input = ostendo::Menu(win, options);
    std::map<std::string, void (*)()>::iterator it = games.find(input);
    if (it != games.end()) {
      it->second();
    }
  }
}
