#include <appareo.h>
#include "game_files/core.hpp"

int main(int argc, char const* argv[]) {
  appareo::InitializeAppareo(true);
  halfdelay(1);
  game::MainMenu();
  appareo::TerminateAppareo();
  return 0;
}
