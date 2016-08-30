#include "..\\SmallWindows\\SmallWindows.h"
#include "EmptyWindow.h"

void MainWindow(vector<String> /* argumentList */,
                WindowShow windowShow) {
  Application::ApplicationName() = TEXT("Empty");
  Application::MainWindowPtr() = new EmptyWindow(windowShow);
}
