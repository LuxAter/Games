#include <pessum.h>
#include <ostendo.h>
#include <iostream>
#include <string>
#include <vector>
#include "games/game_headers.hpp"
#include <map>

void Test(){

}

int main(int argc, char const* argv[]) {
  srand(time(NULL));
  ostendo::InitOstendo(true, 1);
  std::map<std::string, void(*)()> games;
  std::vector<std::string> options;
  /*Add games here*/
  games["Snake"] = games::snake::Game;
  games["Pac Man"] = Test;
  /*--------------*/
  for(std::map<std::string, void(*)()>::iterator it = games.begin(); it != games.end(); ++it){
    options.push_back(it->first);
  }
  options.push_back("Quit");
  std::string input = "";
  while(input != "Quit"){
    ostendo::Window win(0.75, 0.5, 0.125, 0.25);
    win.ToggleBorder();
    win.ToggleTitle("OpenSource-Games");
    input = ostendo::Menu(win, options);
    win.DelWin();
    if(input != "Quit"){
      std::map<std::string, void(*)()>::iterator it = games.find(input);
      if(it != games.end()){
        it->second();
      }
    }
  }
  ostendo::TermOstendo();
  pessum::SaveLog("out.log");
  return 0;
}
