enum Direction { North, East, South, West };
typedef pair<int, int> IntPair;

class TetrisFigure {
  public:
    TetrisFigure();

  protected:
    TetrisFigure(Window* windowPtr, GameGrid* colorGridPtr,
           Color color, int row, int col, Direction direction,
           IntPair* northList, IntPair* eastList,
           IntPair* southList, IntPair* westList);
  public:
    TetrisFigure& operator=(const TetrisFigure& figure);

    void TryMoveLeft();
    void TryMoveRight();
    void TryRotateClockwise();
    void TryRotateAnticlockwise();
    bool TryMoveDown();

    bool IsFigureValid();
    static bool IsFigureValid(int direction, int row, int col,
                  GameGrid* gameGridPtr, IntPair* figureInfo[]);
    static bool IsSquareValid(int row, int col,
                              GameGrid* gameGridPtr); 
    void AddToGrid();

    void InvalidateFigure(Size offsetSize = ZeroSize);
    void DrawFigure(Graphics& graphics,
                    Size offsetSize = ZeroSize) const;
 
  protected:    
    Window* windowPtr;
    GameGrid* gameGridPtr;
    Color color;
    int row, col;
    Direction direction;
    IntPair* figureInfo[4];
};