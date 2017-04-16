#include <ostendo.h>
#include "game_files/core.hpp"

int main(int argc, char const* argv[]) {
  ostendo::InitOstendo(true, 1);
  game::MainMenu();
  ostendo::TermOstendo();
  return 0;
}
