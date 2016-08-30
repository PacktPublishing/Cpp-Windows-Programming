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

void MainWindow(vector<String> /* argumentList */,
                WindowShow windowShow) {
  Application::ApplicationName() = TEXT("Tetris");
  Application::MainWindowPtr() = new TetrisWindow(windowShow);
}