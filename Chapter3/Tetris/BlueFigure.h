class BlueFigure : public TetrisFigure {
  public:
    static IntPair NorthList[], EastList[],
           SouthList[], WestList[];
    BlueFigure(Window* windowPtr, GameGrid* gameGridPtr);
};