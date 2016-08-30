#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

void MainWindow(vector<String> /* argumentList */,
  WindowShow windowShow) {
  Application::ApplicationName() = TEXT("Word");
  Application::MainWindowPtr() = new WordDocument(windowShow);
}