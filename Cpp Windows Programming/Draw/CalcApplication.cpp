#include "..\\SmallWindows\\SmallWindows.h"
#include "Figure.h"
#include "Line.h"
#include "Arrow.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Text.h"
#include "DrawWindow.h"

int WINAPI WinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PSTR stCmdLine, int iCmdShow) {
  CalcWindow calcWindow(hInstance, iCmdShow);
  return calcWindow.Run();
}
