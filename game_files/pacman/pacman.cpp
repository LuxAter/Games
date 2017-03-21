#include <appareo.h>
#include <pessum.h>
#include "pacman.hpp"

using namespace appareo;
using namespace appareo::curse;
using namespace appareo::curse::out;
using namespace appareo::induco;

namespace pacman {
  std::vector<std::vector<int>> grid;
}

void pacman::Game() {}

void pacman::LoadGrid() {
  grid.clear();
  std::ifstream load_grid("pacman_grid.txt");
  if (load_grid.is_open()) {
    std::string in;
    while (getline(load_grid, in)) {
      std::vector<int> row;
      for (int i = 0; i < in.size(); i++) {
        row.push_back(int(in[i]) - 48);
      }
      grid.push_back(row);
    }
  }
}
