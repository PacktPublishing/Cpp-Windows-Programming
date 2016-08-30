#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "BrownFigure.h"

IntPair BrownFigure::HorizontalList[] =
                     {IntPair(-1,0), IntPair(1,0), IntPair(2,0)},
        BrownFigure::VerticalList[] =
                     {IntPair(0,-1), IntPair(0,1), IntPair(0,2)};

BrownFigure::BrownFigure(Window* windowPtr, GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Brown, 1, (Cols / 2) - 1,
               North, HorizontalList, VerticalList,
               HorizontalList, VerticalList) {
  // Empty.
}