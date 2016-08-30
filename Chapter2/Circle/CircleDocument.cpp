#include "..\\SmallWindows\\SmallWindows.h"
#include "Circle.h"
#include "CircleDocument.h"

CircleDocument::CircleDocument(WindowShow windowShow)
 :StandardDocument(LogicalWithScroll, USLetterPortrait,
                   TEXT("Circle Files, cle"), nullptr,
                   OverlappedWindow, windowShow) {
  Menu menuBar(this);
  menuBar.AddMenu(StandardFileMenu(false));

  Menu colorMenu(this, TEXT("&Color"));
  colorMenu.AddItem(TEXT("&Red\tCtrl+R"), OnRed,
                    nullptr, nullptr, RedRadio);
  colorMenu.AddItem(TEXT("&Green\tCtrl+G"), OnGreen,
                    nullptr, nullptr, GreenRadio);
  colorMenu.AddItem(TEXT("&Blue\tCtrl+B"), OnBlue,
                    nullptr, nullptr, BlueRadio);
  colorMenu.AddSeparator();
  colorMenu.AddItem(TEXT("&Dialog ..."), OnColorDialog);
  menuBar.AddMenu(colorMenu);

  menuBar.AddMenu(StandardHelpMenu());
  SetMenuBar(menuBar);

  nextColor.ReadColorFromRegistry(TEXT("NextColor"), Red);
}

CircleDocument::~CircleDocument() {
  nextColor.WriteColorToRegistry(TEXT("NextColor"));
}

void CircleDocument::ClearDocument() {
  circleList.clear();
}

bool CircleDocument::WriteDocumentToStream(String name,
                          ostream& outStream) const {
  int size = circleList.size();
  outStream.write((char*) &size, sizeof size);

  for (Circle circle : circleList) {
    circle.WriteCircle(outStream);
  }

  return ((bool) outStream);
}

bool CircleDocument::ReadDocumentFromStream(String name,
                                 istream& inStream) {
  int size;
  inStream.read((char*) &size, sizeof size);

  for (int count = 0; count < size; ++count) {
    Circle circle;
    circle.ReadCircle(inStream);
    circleList.push_back(circle);
  }

  return ((bool) inStream);
}

void CircleDocument::OnMouseDown
           (MouseButton mouseButtons, Point mousePoint,
            bool shiftPressed /* = false */,
            bool controlPressed /* = false */) {
  if (mouseButtons == LeftButton) {
    moveIndex = -1;
    int size = circleList.size();

    for (int index = 0; index < size; ++index) {
      if (circleList[index].IsClick(mousePoint)) {
        moveIndex = index;
        movePoint = mousePoint;
        break;
      }
    }

    if (moveIndex == -1) {
      Circle newCircle(mousePoint, CircleRadius,
                       nextColor);
      circleList.push_back(newCircle);
      Invalidate(newCircle.Area());
      UpdateWindow();
      SetDirty(true);
    }
  }
}

void CircleDocument::OnMouseMove
           (MouseButton mouseButtons, Point mousePoint,
            bool shiftPressed /* = false */,
            bool controlPressed /* = false */) {
  if ((mouseButtons == LeftButton)&&(moveIndex != -1)) {
    Size distanceSize = mousePoint - movePoint;
    movePoint = mousePoint;

    Circle& movedCircle = circleList[moveIndex];
    Invalidate(movedCircle.Area());
    movedCircle.Center() += distanceSize;
    Invalidate(movedCircle.Area());

    UpdateWindow();
    SetDirty(true);
  }
}

void CircleDocument::OnMouseUp
           (MouseButton mouseButtons, Point mousePoint,
            bool shiftPressed /* = false */,
            bool controlPressed /* = false */) {
  moveIndex = -1;
}

void CircleDocument::OnDraw(Graphics& graphics,
                            DrawMode /* drawMode */) const {
  for (Circle circle : circleList) {
    circle.Draw(graphics);
  }
}

bool CircleDocument::RedRadio() const {
  return (nextColor == Red);
}

bool CircleDocument::GreenRadio() const {
  return (nextColor == Green);
}

bool CircleDocument::BlueRadio() const {
  return (nextColor == Blue);
}

void CircleDocument::OnRed() {
  nextColor = Red;
}

void CircleDocument::OnGreen() {
  nextColor = Green;
}

void CircleDocument::OnBlue() {
  nextColor = Blue;
}

void CircleDocument::OnColorDialog() {
  StandardDialog::ColorDialog(this, nextColor);
}