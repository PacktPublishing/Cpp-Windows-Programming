#include "..\\SmallWindows\\SmallWindows.h"
#include "GameGrid.h"
#include "TetrisFigure.h"
#include "RedFigure.h"
#include "BrownFigure.h"
#include "TurquoiseFigure.h"
#include "GreenFigure.h"
#include "YellowFigure.h"
#include "BlueFigure.h"
#include "PurpleFigure.h"
#include "TetrisWindow.h"

TetrisWindow::TetrisWindow(WindowShow windowShow)
 :Window(PreviewCoordinate, Rect(0, 0, 100, 100),
         nullptr, OverlappedWindow, NoStyle, Normal),
  gameGrid(Rect(0, 20, 100, 100)) {
  SetHeader(TEXT("Tetris"));

  SetTimer(TimerId, 1000);
  srand((unsigned int) time(nullptr));

  figureList.push_back(RedFigure(this, &gameGrid));
  figureList.push_back(BrownFigure(this, &gameGrid));
  figureList.push_back(TurquoiseFigure(this, &gameGrid));
  figureList.push_back(GreenFigure(this, &gameGrid));
  figureList.push_back(YellowFigure(this, &gameGrid));
  figureList.push_back(BlueFigure(this, &gameGrid));
  figureList.push_back(PurpleFigure(this, &gameGrid));

  fallingFigure = figureList[rand() % figureList.size()];
  nextFigure = figureList[rand() % figureList.size()];
}

TetrisWindow::~TetrisWindow() {
  DropTimer(TimerId);
}

bool TetrisWindow::OnKeyDown(WORD key, bool /* shiftPressed */,
                             bool /* controlPressed */) {
  switch (key) {
    case KeyLeft:
      fallingFigure.TryMoveLeft();
      break;

    case KeyRight:
      fallingFigure.TryMoveRight();
      break;

    case KeyUp:
      fallingFigure.TryRotateAnticlockwise();
      break;

    case KeyDown:
      fallingFigure.TryRotateAnticlockwise();
      break;

    case KeySpace:
      while (fallingFigure.TryMoveDown()) {
        ::Sleep(10);
      }
      break;
  }

  return true;
}

void TetrisWindow::OnDraw(Graphics& graphics,
                          DrawMode /* drawMode */) const {
  gameGrid.DrawGameGrid(graphics, inverse);

  graphics.FillRectangle(Rect(Point(0, 0), Point(100,20)), White, White);
  graphics.DrawLine(Point(40, 0), Point(40, 20), Black);
  graphics.DrawLine(Point(0, 20), Point(100, 20), Black);

  Font scoreFont(TEXT("Times New Roman"), 10);
  graphics.DrawText(Rect(0, 0, 40, 20), to_String(currScore),
                    scoreFont, Black, White, false);

  fallingFigure.DrawFigure(graphics);
  nextFigure.DrawFigure(graphics, Size(25, -18));
}

void TetrisWindow::OnGainFocus() {
  SetTimer(TimerId, 1000);
}

void TetrisWindow::OnLoseFocus() {
  DropTimer(TimerId);
}

void TetrisWindow::OnTimer(int /* timerId */) {
  if (timerActive) {
    if (!fallingFigure.TryMoveDown()) {
      EndOfFigure();
    }
  }
}

void TetrisWindow::EndOfFigure() {
  fallingFigure.AddToGrid();
  fallingFigure = nextFigure;
  fallingFigure.InvalidateFigure();

  nextFigure = figureList[rand() % figureList.size()];
  Rect nextArea(40, 0, 100, 20);
  Invalidate(nextArea);
  UpdateWindow();

  DeleteFullRows();
  UpdateWindow();

  if (!fallingFigure.IsFigureValid()) {
    GameOver();
  }
}

void TetrisWindow::GameOver() {
  Invalidate();
  UpdateWindow();

  timerActive = false;
  String message = TEXT("Game Over.\nYou scored ") +
                   to_String(currScore) +
                   TEXT(" points.\nAnother game?");

  if (MessageBox(message, TEXT("Tetris"), YesNo, Question) == Yes) {
    NewGame();
  }
  else {
    PostQuitMessage(0);
  }
}

void TetrisWindow::NewGame() {
  fallingFigure = figureList[rand() % figureList.size()];
  nextFigure = figureList[rand() % figureList.size()];

  currScore = 0;
  gameGrid.ClearGameGrid();
  timerActive = true;
  Invalidate();
  UpdateWindow();
}

void TetrisWindow::DeleteFullRows() {
  int row = Rows - 1;

  while (row >= 0) {
    if (IsRowFull(row)) {
      FlashRow(row);
      DeleteRow(row);

      ++currScore;
      Rect scoreArea(0, 0, 40, 20);
      Invalidate(scoreArea);
      UpdateWindow();
    }
    else {
      --row;
    }
  }
}

bool TetrisWindow::IsRowFull(int row) {
  for (int col = 0; col < Cols; ++col) {
    if (gameGrid[row][col] == White) {
      return false;
    }
  }

  return true;
}

void TetrisWindow::FlashRow(int row) {
  Rect gridArea = gameGrid.GridArea();
  int colWidth = gridArea.Width() / Cols,
      rowHeight = gridArea.Height() / Rows;

  Rect rowArea(0, row * rowHeight, Cols * colWidth,
               (row + 1) * rowHeight);

  for (int count = 0; count < 3; ++count) {
    inverse = true;
    Invalidate(rowArea + gridArea.TopLeft());
    UpdateWindow();
    ::Sleep(50);

    inverse = false;
    Invalidate(rowArea + gridArea.TopLeft());
    UpdateWindow();
    ::Sleep(50);
  }
}

void TetrisWindow::DeleteRow(int markedRow) {
  for (int row = markedRow; row > 0; --row) {
    for (int col = 0; col < Cols; ++col) {
      gameGrid[row][col] = gameGrid[row - 1][col];
    }
  }

  for (int col = 0; col < Cols; ++col) {
    gameGrid[0][col] = White;
  }

  Invalidate(gameGrid.GridArea());
  UpdateWindow();
}

bool TetrisWindow::TryClose() {
  timerActive = false;

  if (MessageBox(TEXT("Quit?"), TEXT("Tetris"),
                 YesNo, Question, true) == Yes) {
    return true;
  }

  timerActive = true;
  return false;
}