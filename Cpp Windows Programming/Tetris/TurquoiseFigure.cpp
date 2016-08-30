#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "TurquoiseFigure.h"

IntPair TurquoiseFigure::HorizontalList[] =
                 {IntPair(-1,0), IntPair(0,1), IntPair(1,1)},
        TurquoiseFigure::VerticalList[] =
                 {IntPair(1,-1), IntPair(1,0), IntPair(0,1)};

TurquoiseFigure::TurquoiseFigure(Window* windowPtr,
                                 GameGrid* gameGridPtr)
 :TetrisFigure(windowPtr, gameGridPtr, Turquoise, 1, (Cols/2) - 1,
               North, HorizontalList, VerticalList,
               HorizontalList, VerticalList) {
  // Empty.
}
