class GreenFigure : public TetrisFigure {
  public:
    static IntPair HorizontalList[], VerticalList[];
    GreenFigure(Window* windowPtr, GameGrid* gameGridPtr);
};
