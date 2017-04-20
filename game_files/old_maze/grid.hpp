#ifndef MAZE_GRID
#define MAZE_GRID
#include <vector>
namespace maze {
  class Grid {
   public:
    void Init(int sizex, int sizey, bool winc = true);
    bool CheckPos(int x, int y);
    void Show();
    void ShowPos(int x, int y);
    void ErasePos(int x, int y);
    void SetDisp(int x, int y);
    void SetPos(int x, int y);
    void Delete();
    int width, height, score, win;
    std::vector<std::vector<int>> GetIntVec();

   private:
    bool wincheck = true;
    int countwin, dispx = 0, dispy = 0, lastdispx = -1, lastdispy = -1;
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> lastdispgrid;
    void GenGrid();
  };
}
#endif
