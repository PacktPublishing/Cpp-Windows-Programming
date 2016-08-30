#include "..\\SmallWindows\\SmallWindows.h"
#include "Circle.h"
#include "CircleDocument.h"

void MainWindow(vector<String> /* argumentList */,
                WindowShow windowShow) {
  Application::ApplicationName() = TEXT("Circle");
  Application::MainWindowPtr() =
    new CircleDocument(windowShow);
}
