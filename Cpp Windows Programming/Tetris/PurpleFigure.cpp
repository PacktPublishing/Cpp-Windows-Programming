#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "PurpleFigure.h"

IntPair PurpleFigure::NorthList[] =
                      {IntPair(-1,0),IntPair(0,1),IntPair(0,2)},
        PurpleFigure::EastList[] =
                      {IntPair(1,0), IntPair(2,0), IntPair(0,1)},
        PurpleFigure::SouthList[] =
                      {IntPair(0,-2),IntPair(0,-1),IntPair(1,0)},
        PurpleFigure::WestList[] =
                      {IntPair(0,-1),IntPair(-2,0),IntPair(-1,0)};

PurpleFigure::PurpleFigure(Window* windowPtr,
                           GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Purple, 1, Cols / 2, South,
               NorthList, EastList, SouthList, WestList) {
  // Empty.
}