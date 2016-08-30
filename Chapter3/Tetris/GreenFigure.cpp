#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "GreenFigure.h"

IntPair GreenFigure::HorizontalList[] =
             {IntPair(1,-1), IntPair(0,-1), IntPair(-1,0)},
        GreenFigure::VerticalList[] =
             {IntPair(0,-1), IntPair(1,0), IntPair(1,1)};

GreenFigure::GreenFigure(Window* windowPtr, GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Green, 1, Cols / 2, North,
               HorizontalList, VerticalList,
               HorizontalList, VerticalList) {
  // Empty.
}
