#include <appareo.h>
#include "game_files/core.hpp"

int main(int argc, char const* argv[]) {
  appareo::InitializeAppareo(true);
  setlocale(LC_ALL, "");
  timeout(1);
  game::MainMenu();
  appareo::TerminateAppareo();
  return 0;
}
