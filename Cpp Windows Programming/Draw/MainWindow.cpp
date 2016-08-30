#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"
#include "LineFigure.h"
#include "ArrowFigure.h"
#include "RectangleFigure.h"
#include "EllipseFigure.h"
#include "DrawDocument.h"

void MainWindow(vector<String> /* argumentList */,
                WindowShow windowShow) {
  Application::ApplicationName() = TEXT("DrawFigure");
  Application::MainWindowPtr() = new DrawDocument(windowShow);
}
