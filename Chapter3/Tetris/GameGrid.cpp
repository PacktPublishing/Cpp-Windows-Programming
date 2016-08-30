#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"

GameGrid::GameGrid(Rect gridArea)
 :gridArea(gridArea) {
  ClearGameGrid();
}

void GameGrid::ClearGameGrid() {
  for (int row = 0; row < Rows; ++row) {
    for (int col = 0; col < Cols; ++col) {
      gameGrid[row][col] = White;
    }
  }
}

void GameGrid::DrawGameGrid(Graphics& graphics, bool inverse)
                            const {
  for (int row = 0; row < Rows; ++row) {
    for (int col = 0; col < Cols; ++col) {
      Color squareColor = gameGrid[row][col];
      Color penColor = (squareColor == White) ? White : Black;
      Color brushColor = inverse ? squareColor.Inverse()
                                 : squareColor;
      DrawSquare(graphics, row, col, penColor, brushColor);
    }
  }
}

void GameGrid::InvalidateSquare(Window* windowPtr, int row,
                                int col, Size offsetSize) {
  int colWidth = gridArea.Width() / Cols,
      rowHeight = gridArea.Height() / Rows;

  Rect squareArea(col * colWidth, row * rowHeight,
                  (col + 1) * colWidth, (row + 1) * rowHeight);
  windowPtr->Invalidate(gridArea.TopLeft() + squareArea +
                        offsetSize);
}

void GameGrid::DrawSquare(Graphics& graphics, int row, int col,
                          Color penColor, Color brushColor,
                          Size offsetSize /* = ZeroSize */) const{
  int colWidth = gridArea.Width() / Cols,
      rowHeight = gridArea.Height() / Rows;

  Rect squareArea (col * colWidth, row * rowHeight,
                  (col + 1) * colWidth, (row + 1) * rowHeight);
  graphics.FillRectangle(gridArea.TopLeft() + squareArea +
                         offsetSize, penColor, brushColor);
}