#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "YellowFigure.h"

IntPair YellowFigure::NorthList[] =
              {IntPair(0,-1), IntPair(-1,0), IntPair(0,1)},
        YellowFigure::EastList[] =
              {IntPair(-1,0),IntPair(0,1),IntPair(1,0)},
        YellowFigure::SouthList[] =
              {IntPair(0,-1),IntPair(1,0),IntPair(0,1)},
        YellowFigure::WestList[] =
              {IntPair(-1,0),IntPair(0,-1),IntPair(1,0)};

YellowFigure::YellowFigure(Window* windowPtr,
                           GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Yellow, 1, (Cols / 2) - 1,
               South, NorthList, EastList, SouthList, WestList) {
  // Empty.
}