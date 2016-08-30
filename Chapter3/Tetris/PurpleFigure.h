class PurpleFigure : public TetrisFigure {
  public:
    static IntPair NorthList[], EastList[],
                   SouthList[], WestList[];
    PurpleFigure(Window* windowPtr, GameGrid* gameGridPtr);
};