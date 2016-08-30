#include "SmallWindows.h"

int WINAPI WinMain(HINSTANCE instanceHandle,
                   HINSTANCE /* prevInstanceHandle */,
                   char* commandLine, int commandShow) {
  Application::RegisterWindowClasses(instanceHandle);
  vector<String> argumentList =
    Split(CharPtrToGenericString(commandLine));
  MainWindow(argumentList, (WindowShow) commandShow);
  return Application::RunMessageLoop();
}