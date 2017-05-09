#include "snake.hpp"
#include <ostendo.h>
#include <pessum.h>
#include <ncurses.h>

using namespace ostendo;

namespace games {
namespace snake {
std::vector<std::vector<int>> grid, last;
}
}


games::snake::Snake::Snake(int c, bool ai){
  color = c;
  head = FindPos();
  ai_controled = ai;
  move_dir = std::make_pair(0,0);
}

games::snake::Snake::~Snake(){
  body.clear(); 
}

void games::snake::Snake::Move() {
  if (dead == false) {
    if (grow == 0 && body.size() > 0) {
      grid[body.end()->first][body.end()->second] = 0;
    } else if (grow > 0) {
      grow--;
      body.insert(body.begin(), head);
    }
    if (body.size() == 0) {
      grid[head.first][head.second] = 76;
    }
    std::pair<int, int> last_head = head;
    head.first += move_dir.first;
    head.second += move_dir.second;
    if (head.first < 0 || head.second < 0 || head.first >= grid.size() ||
        head.second >= grid[0].size()) {
      dead = true;
    } else if (grid[head.first][head.second] > 0) {
      if(head != last_head){
      dead = true;
      }
    } else if (grid[head.first][head.second] < 0) {
      grow += rate;
    } else {
      grid[head.first][head.second] = color;
    }
  }
}

void games::snake::Snake::SetDir(int key) {
  if (ai_controled == false) {
    if (key == controls[0]) {
      move_dir = std::make_pair(0, -1);
    } else if (key == controls[1]) {
      move_dir = std::make_pair(-1, 0);
    } else if (key == controls[2]) {
      move_dir = std::make_pair(0, 1);
    } else if (key == controls[3]) {
      move_dir = std::make_pair(1, 0);
    }
  } else if (ai_controled == true && dead == false){
    AiMove();
  }
}

void games::snake::Snake::AiMove(){

}





void games::snake::Game() {
  bool running = true;
  std::vector<Var> fields = {
      Var("Snakes", 1),          Var("Food", 1),
      Var("Obsticals", 0),       Var("Growth Rate", 5),
      Var("Movement Speed", 15), Var("Default Size", true)};
  while (running == true) {
    ostendo::Window win(0.5, 0.5, 0.25, 0.25);
    win.ToggleBorder();
    win.ToggleTitle("Snake");
    fields = Form(win, fields);
    if (fields[5].Bool() == false) {
      std::vector<Var> window_settings = {Var("Width", 20), Var("Height", 20)};
      window_settings = Form(win, window_settings);
    }
    win.NewWindow(1.0, 1.0, 0, 0);
  
    int temp = Run(win, fields[2].Int(), fields[1].Int(), fields[0].Int(), fields[3].Int(),
        fields[4].Int());
    if(temp == 2){
      running = false;
    }
  }
}

int games::snake::Run(ostendo::Window win, int c_wall, int c_food,
                       int c_snakes, int growth, int speed) {
  grid.clear();
  grid = std::vector<std::vector<int>>(win.window_space.w,
                                       std::vector<int>(win.window_space.h));
  last = grid; 
  while(c_wall > 0){
    std::pair<int, int> pos = FindPos();
    grid[pos.first][pos.second] = 1;
    c_wall--;
  }
  std::vector<Snake> snakes;
  snakes.push_back(Snake(2, false));
  while(c_snakes > 0){
    snakes.push_back(Snake(rand() % 100, true));
        c_snakes--;
  }
  int running = 1, counter = 0;
  while(running == 1){
    counter++;
    DisplayGrid(win);
    int input = getch();
    for(int i = 0; i < snakes.size(); i++){
      snakes[i].SetDir(input);
    }
    if(counter >= 1000 / speed){
      for(int i = 0; i < snakes.size(); i++){
        snakes[i].Move();
      }
      counter = 0;
    }
    if(input == int('q')){
      running = 2;
    }else if(input == 27){
      running = 3;
    }
  }
  win.DelWin();
  return(running);
}

std::pair<int, int> games::snake::FindPos(){
  bool searching = true;
  std::pair<int, int> pos = std::make_pair(-1, -1);
  while(searching == true){
    pos.first = rand() % grid.size();
    pos.second = rand() % grid[0].size();
    if(grid[pos.first][pos.second] == 0){
      searching = false;
    }
  }
  return(pos);
}

void games::snake::DisplayGrid(ostendo::Window win){
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].size(); j++) {
      if (grid[i][j] != last[i][j]) {
        last[i][j] = grid[i][j];
        win.SetCurs(j + 1, i + 1);
        if(grid[i][j] >= 10){
          win.Print("#o%i+#f%i", grid[i][j], grid[i][j]);
        }else if(grid[i][j] == 1){
          win.Print("#o07 #f07");
        }else if(grid[i][j] == 0){
          win.Print(" ");
        }
      }
    }
  }
}
