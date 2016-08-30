#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"

TetrisFigure::TetrisFigure() {
  // Empty
}

TetrisFigure::TetrisFigure(Window*windowPtr, GameGrid*gameGridPtr,
                           Color color, int row, int col,
                           Direction direction,
                           IntPair* northList, IntPair* eastList,
                           IntPair* southList, IntPair* westList)
 :windowPtr(windowPtr),
  gameGridPtr(gameGridPtr),
  color(color),
  row(row),
  col(col),
  direction(direction) {
  figureInfo[North] = northList;
  figureInfo[East] = eastList;
  figureInfo[South] = southList;
  figureInfo[West] = westList;
}

TetrisFigure& TetrisFigure::operator=(const TetrisFigure& figure){
  if (this != &figure) {
    windowPtr = figure.windowPtr;
    gameGridPtr = figure.gameGridPtr;
    color = figure.color;
    row = figure.row;
    col = figure.col;
    direction = figure.direction;
    figureInfo[North] = figure.figureInfo[North];
    figureInfo[East] = figure.figureInfo[East];
    figureInfo[South] = figure.figureInfo[South];
    figureInfo[West] = figure.figureInfo[West];
  }

  return *this;
}

void TetrisFigure::TryMoveLeft() {
  if (IsFigureValid(direction, row, col - 1, gameGridPtr, figureInfo)) {
    InvalidateFigure();
    --col;
    InvalidateFigure();
    windowPtr->UpdateWindow();
  }
}

void TetrisFigure::TryMoveRight() {
  if (IsFigureValid(direction, row, col + 1, gameGridPtr, figureInfo)) {
    InvalidateFigure();
    ++col;
    InvalidateFigure();
    windowPtr->UpdateWindow();
  }
}

void TetrisFigure::TryRotateClockwise() {
  Direction newDirection = (direction == West) ? North :
                           ((Direction) (direction + 1));

  if (IsFigureValid(newDirection, row, col,
                    gameGridPtr, figureInfo)) {
    InvalidateFigure();
    direction = newDirection;
    InvalidateFigure();
    windowPtr->UpdateWindow();
  }
}

void TetrisFigure::TryRotateAnticlockwise() {
  Direction newDirection = (this->direction == North) ? West :
                           ((Direction) (direction - 1));

  if (IsFigureValid(newDirection, row, col,
                    gameGridPtr, figureInfo)) {
    InvalidateFigure();
    direction = newDirection;
    InvalidateFigure();
    windowPtr->UpdateWindow();
  }
}

bool TetrisFigure::TryMoveDown() {
  if (IsFigureValid(direction, row + 1, col, gameGridPtr, figureInfo)) {
    InvalidateFigure();
    ++row;
    InvalidateFigure();
    windowPtr->UpdateWindow();
    return true;
  }

  return false;
}

bool TetrisFigure::IsFigureValid() {
  return IsFigureValid(direction, row, col,
                       gameGridPtr, figureInfo);
}

bool TetrisFigure::IsFigureValid(int direction, int row, int col,
                                 GameGrid* gameGridPtr,
                                 IntPair* figureInfo[]) {
  int relRow0 = row + figureInfo[direction][0].first,
      relCol0 = col + figureInfo[direction][0].second,
      relRow1 = row + figureInfo[direction][1].first,
      relCol1 = col + figureInfo[direction][1].second,
      relRow2 = row + figureInfo[direction][2].first,
      relCol2 = col + figureInfo[direction][2].second;

  return IsSquareValid(row, col, gameGridPtr) &&
         IsSquareValid(relRow0, relCol0, gameGridPtr) &&
         IsSquareValid(relRow1, relCol1, gameGridPtr) &&
         IsSquareValid(relRow2, relCol2, gameGridPtr);
}

bool TetrisFigure::IsSquareValid(int row, int col,
                                 GameGrid* gameGridPtr) {
  return (row >= 0) && (row < Rows) &&
         (col >= 0) && (col < Cols) &&
         ((*gameGridPtr)[row][col] == White);
}

void TetrisFigure::AddToGrid() {
  (*gameGridPtr)[row][col] = color;

  { int relRow = row + figureInfo[direction][0].first,
        relCol = col + figureInfo[direction][0].second;
    (*gameGridPtr)[relRow][relCol] = color;
  }

  { int relRow = row + figureInfo[direction][1].first,
        relCol = col + figureInfo[direction][1].second;
    (*gameGridPtr)[relRow][relCol] = color;
  }

  { int relRow = row + figureInfo[direction][2].first,
        relCol = col + figureInfo[direction][2].second;
    (*gameGridPtr)[relRow][relCol] = color;
  }
}

void TetrisFigure::InvalidateFigure(Size offsetSize/*=ZeroSize*/){
  gameGridPtr->InvalidateSquare(windowPtr, row, col, offsetSize);

  { int relRow = row + figureInfo[direction][0].first,
        relCol = col + figureInfo[direction][0].second;
    gameGridPtr->InvalidateSquare(windowPtr, relRow,
                                  relCol, offsetSize);
  }

  { int relRow = row + figureInfo[direction][1].first,
        relCol = col + figureInfo[direction][1].second;
    gameGridPtr->InvalidateSquare(windowPtr, relRow,
                                  relCol, offsetSize);
  }

  { int relRow = row + figureInfo[direction][2].first,
        relCol = col + figureInfo[direction][2].second;
    gameGridPtr->InvalidateSquare(windowPtr, relRow,
                                  relCol, offsetSize);
  }
}

void TetrisFigure::DrawFigure(Graphics& graphics, Size offsetSize)
                              const {
  gameGridPtr->DrawSquare(graphics, row, col,
                          Black, color, offsetSize);

  { int relRow = row + figureInfo[direction][0].first,
        relCol = col + figureInfo[direction][0].second;
    gameGridPtr->DrawSquare(graphics, relRow, relCol,
                            Black, color, offsetSize);
  }

  { int relRow = row + figureInfo[direction][1].first,
        relCol = col + figureInfo[direction][1].second;
    gameGridPtr->DrawSquare(graphics, relRow, relCol,
                            Black, color, offsetSize);
  }

  { int relRow = row + figureInfo[direction][2].first,
        relCol = col + figureInfo[direction][2].second;
    gameGridPtr->DrawSquare(graphics, relRow, relCol,
                            Black, color, offsetSize);
  }
}