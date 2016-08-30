class YellowFigure : public TetrisFigure {
  public:
    static IntPair NorthList[], EastList[],
                   SouthList[], WestList[];
    YellowFigure(Window* windowPtr, GameGrid* gameGridPtr);
};
