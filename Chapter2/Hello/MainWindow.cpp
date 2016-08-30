#include "..\\SmallWindows\\SmallWindows.h"
#include "HelloWindow.h"

void MainWindow(vector<String> /* argumentList */,
                WindowShow windowShow) {
  Application::ApplicationName() = TEXT("Hello");
  Application::MainWindowPtr() =
    new HelloWindow(windowShow);
}
