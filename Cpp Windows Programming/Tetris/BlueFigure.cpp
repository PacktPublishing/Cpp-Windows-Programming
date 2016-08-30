#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "BlueFigure.h"

IntPair BlueFigure::NorthList[] =
                    {IntPair(0,-2),IntPair(0,-1),IntPair(-1,0)},
        BlueFigure::EastList[] =
                    {IntPair(-2,0), IntPair(-1,0), IntPair(0,1)},
        BlueFigure::SouthList[] =
                    {IntPair(1,0), IntPair(0,1), IntPair(0,2)},
        BlueFigure::WestList[] =
                    {IntPair(0,-1), IntPair(1,0), IntPair(2,0)};

BlueFigure::BlueFigure(Window* windowPtr, GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Blue, 1, (Cols / 2) - 1,
               South, NorthList, EastList, SouthList, WestList) {
  // Empty.
}