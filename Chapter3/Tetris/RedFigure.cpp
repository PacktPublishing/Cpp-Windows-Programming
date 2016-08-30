#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "RedFigure.h"

IntPair RedFigure::GenericList[] =
                   {IntPair(0,1), IntPair(1,0), IntPair(1,1)};

RedFigure::RedFigure(Window* windowPtr, GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Red, 1, (Cols / 2) - 1,
               North, GenericList, GenericList, GenericList,
               GenericList) {
  // Empty.
}