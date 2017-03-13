#include <appareo.h>
#include <pessum.h>
#include <vector>
#include "tetris.hpp"
#include "tetris_class.hpp"

void tetris::Tetrimino::Gen() {
  center_pos = std::make_pair(grid.size() / 2, 1);
  init = true;
  shape = rand() % 7;
  if (shape == 0) {
    blocks = {-1, 0, 0, 0, 1, 0, 2, 0};
    center_pointer = 1;
  } else if (shape == 1) {
    blocks = {-1, 0, 0, 0, 1, 0, 1, 1};
    center_pointer = 1;
    center_pos.second--;
  } else if (shape == 2) {
    blocks = {-1, 0, 0, 0, 1, 0, -1, 1};
    center_pointer = 1;
    center_pos.second--;
  } else if (shape == 3) {
    blocks = {-1, -1, 0, -1, 0, 0, -1, 0};
    center_pointer = 2;
  } else if (shape == 4) {
    blocks = {-1, 0, 0, 0, 0, -1, 1, -1};
    center_pointer = 2;
  } else if (shape == 5) {
    blocks = {-1, 0, 0, 0, 1, 0, 0, -1};
    center_pointer = 1;
  } else if (shape == 6) {
    blocks = {-1, 0, 0, 0, 0, 1, 1, 1};
    center_pointer = 2;
    center_pos.second--;
  }
}

void tetris::Tetrimino::Del() {
  Display();
  blocks.clear();
  shape = -1;
  center_pointer = -1;
  init = false;
}

bool tetris::Tetrimino::Move(int dir, int max) {
  bool at_max = false;
  for (int i = 0; i < blocks.size(); i += 2) {
    if (dir == UP && (center_pos.second + blocks[i + 1] == max ||
                      grid[center_pos.first + blocks[i]]
                          [center_pos.second + blocks[i + 1] - 1] != 0)) {
      at_max = true;
    } else if (dir == DOWN &&
               (center_pos.second + blocks[i + 1] == max ||
                grid[center_pos.first + blocks[i]]
                    [center_pos.second + blocks[i + 1] + 1] != 0)) {
      at_max = true;
    } else if (dir == LEFT && (center_pos.first + blocks[i] == max ||
                               grid[center_pos.first + blocks[i] - 1]
                                   [center_pos.second + blocks[i + 1]] != 0)) {
      at_max = true;
    } else if (dir == RIGHT && (center_pos.first + blocks[i] == max ||
                                grid[center_pos.first + blocks[i] + 1]
                                    [center_pos.second + blocks[i + 1]] != 0)) {
      at_max = true;
    }
  }
  if (at_max == false) {
    if (dir == UP) {
      center_pos.second--;
    } else if (dir == DOWN) {
      center_pos.second++;
    } else if (dir == LEFT) {
      center_pos.first--;
    } else if (dir == RIGHT) {
      center_pos.first++;
    }
  }
  return (at_max);
}

void tetris::Tetrimino::Rotate() {
  if (shape == 0 || shape == 4 || shape == 6) {
  } else if (shape == 1 || shape == 2 || shape == 5) {
    for (int i = 0; i < blocks.size(); i += 2) {
      if (blocks[i] > 0 && blocks[i + 1] > 0) {
        blocks[i] *= -1;
      } else if (blocks[i] < 0 && blocks[i + 1] > 0) {
        blocks[i + 1] *= -1;
      } else if (blocks[i] < 0 && blocks[i + 1] < 0) {
        blocks[i] *= -1;
      } else if (blocks[i] > 0 && blocks[i + 1] < 0) {
        blocks[i + 1] *= -1;
      } else if (blocks[i] == 0 && blocks[i + 1] > 0){
        
      } else if (blocks[i] == 0 && blocks[i+1] > 0){
      } else if (blocks[i] > 0 && blocks[i+1] == 0){

      }else if(blocks[i] < 0 && blocks[i+1] == 0){
      }
    }
  }
}

void tetris::Tetrimino::Display() {
  for (int i = 0; i < blocks.size(); i += 2) {
    tetris::grid[center_pos.first + blocks[i]]
                [center_pos.second + blocks[i + 1]] = shape + 1;
  }
}
void tetris::Tetrimino::Erase() {
  for (int i = 0; i < blocks.size(); i += 2) {
    tetris::grid[center_pos.first + blocks[i]]
                [center_pos.second + blocks[i + 1]] = 0;
  }
}
