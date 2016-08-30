const int Rows = 20, Cols = 10;

class GameGrid {
  public:
    GameGrid(Rect gridArea);
    void ClearGameGrid();

    Color* operator[](int row) {return gameGrid[row];}
    void InvalidateSquare(Window* windowPtr, int row,
                          int col, Size offsetSize);
    void DrawGameGrid(Graphics& graphics, bool inverse) const;
    void DrawSquare(Graphics& graphics, int row, int col,
                    Color penColor, Color brushColor,
                    Size offsetSize = ZeroSize) const;

    Rect GridArea() const {return gridArea;}

  private:
    Rect gridArea;
    Color gameGrid[Rows][Cols];
};