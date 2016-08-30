#include "..\\SmallWindows\\SmallWindows.h"
#include "HelloWindow.h"

HelloWindow::HelloWindow(WindowShow windowShow)
 :Window(LogicalWithScroll, ZeroSize, nullptr,
         OverlappedWindow, NoStyle, windowShow) {
  SetHeader(TEXT("Hello Window"));
}

void HelloWindow::OnDraw(Graphics& graphics,
                         DrawMode /* drawMode */) const {
  Size clientSize = GetClientSize();
  Rect clientRect(Point(0, 0), clientSize);
  Font textFont(TEXT("Times New Roman"), 12, false, true);
  graphics.DrawText(clientRect, TEXT("Hello, Small Windows!"),
                    textFont , Black, White);
}