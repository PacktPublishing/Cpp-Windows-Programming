#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"
#include "Cell.h"
#include "CalcDocument.h"

void MainWindow(vector<String> /* argumentList */,
                WindowShow windowShow) {
  Application::ApplicationName() = TEXT("Calc");
  Application::MainWindowPtr()= new CalcDocument(windowShow);
}
