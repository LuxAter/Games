#include <appareo.h>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "core.hpp"
#include "game_headers.hpp"

using namespace appareo::curse;

void game::MainMenu() {
  std::map<std::string, void (*)()> games;
  games["FOUR IN A ROW"] = fiar::Game;
  games["MAZE"] = maze::Game;
  games["MINE SWEEPER"] = minesw::Game;
  games["PACMAN"] = pacman::Game;
  games["SNAKE"] = snake::Game;
  games["TETRIS"] = tetris::Game;
  games["TIC TAC TOE"] = ttt::Game;
  std::vector<std::string> options;
  for (std::map<std::string, void (*)()>::iterator it = games.begin();
       it != games.end(); ++it) {
    options.push_back(it->first);
  }
  options.push_back("QUIT");
  std::string input = "";
  while (input != "QUIT") {
    input = appareo::curse::NewMenu(options, "Games", scrwidth / 2,
                                    scrheight / 2)[0];
    std::map<std::string, void (*)()>::iterator it = games.find(input);
    if (it != games.end()) {
      it->second();
    }
  }
}
