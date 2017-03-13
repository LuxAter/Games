#include <appareo.h>
#include <string>
#include <vector>
#include "core.hpp"
#include "game_headers.hpp"

using namespace appareo::curse;

void game::MainMenu() {
  std::vector<std::vector<std::string>> options = {
      {"TIC TAC TOE", "SNAKE", "MAZE", "MINE SWEEPER"}, {"TETRIS", "FOUR IN A ROW"}, {"QUIT"}};
  std::string input = "";
  while (input != "QUIT") {
    input = appareo::curse::NewMenu(options, "Games", scrwidth / 2,
                                    scrheight / 2)[0];
    if (input == "TIC TAC TOE") {
      ttt::Game();
    } else if (input == "SNAKE") {
      snake::Game();
    } else if (input == "MAZE") {
      maze::Game();
    } else if (input == "MINE SWEEPER") {
      minesw::Game();
    } else if (input == "TETRIS") {
      tetris::Game();
    } else if (input == "FOUR IN A ROW"){
      fiar::Game();
    }
  }
}
