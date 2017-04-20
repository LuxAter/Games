#ifndef MINESW_GRID
#define MINESW_GRID
#include <vector>
namespace minesw {
  class Grid {
   public:
    void Init(int sizex, int sizey, int mine);
    bool CheckPos(int x, int y);
    void ClearSpace(int x, int y);
    void Show(int x, int y);
    void Delete();
    void GenMines(int xpos, int ypos);
    void Flag(int x, int y);
    bool Compleated();
    int width, height, minecount, score, flagcount = 0;

   private:
    int win, countwin;
    std::vector<std::vector<bool>> rawgrid;
    std::vector<std::vector<int>> displaygrid, lastgrid;
    void GenGrid();
    int GetCount(int x, int y);
  };
}
#endif
