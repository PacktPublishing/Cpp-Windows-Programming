#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"
#include "LineFigure.h"
#include "ArrowFigure.h"

ArrowFigure::ArrowFigure(const Window* windowPtr)
 :LineFigure(windowPtr) {
  CalculateArrowHead();
}

bool ArrowFigure::IsClick(Point mousePoint) {
  return LineFigure::IsClick(mousePoint) ||
         IsPointInLine(firstPoint, leftPoint, mousePoint) ||
         IsPointInLine(firstPoint, rightPoint, mousePoint) ||
         IsPointInLine(leftPoint, rightPoint, mousePoint);
}

bool ArrowFigure::IsInside(Rect area) {
  return area.PointInside(firstPoint) &&
         area.PointInside(lastPoint) &&
         area.PointInside(leftPoint) &&
         area.PointInside(rightPoint);
}

void ArrowFigure::Move(Size distanceSize) {
  LineFigure::Move(distanceSize);
  leftPoint += distanceSize;
  rightPoint += distanceSize;
}

void ArrowFigure::Modify(Size distanceSize) {
  LineFigure::Modify(distanceSize);
  CalculateArrowHead();
}

void ArrowFigure::DoubleClick(Point mousePoint) {
  if (IsClick(mousePoint)) {
    Invalidate();
    Point tempPoint = firstPoint;
    firstPoint = lastPoint;
    lastPoint = tempPoint;
    CalculateArrowHead();
    Invalidate();
  }
}

Rect ArrowFigure::Area() const {
  Point topLeft(min(firstPoint.X(), min(lastPoint.X(),
                    min(leftPoint.X(), rightPoint.X()))),
                min(firstPoint.Y(), min(lastPoint.Y(),
                    min(leftPoint.Y(), rightPoint.Y())))),
        bottomRight(max(firstPoint.X(), max(lastPoint.X(),
                        max(leftPoint.X(), rightPoint.X()))),
                    max(firstPoint.Y(), max(lastPoint.Y(),
                        max(leftPoint.Y(), rightPoint.Y()))));

  if (IsMarked()) {
    topLeft -= MarkRadius;
    bottomRight += MarkRadius;
  }

  return Rect(topLeft, bottomRight);
}

void ArrowFigure::Draw(Graphics& graphics) const {
  LineFigure::Draw(graphics);

  graphics.DrawLine(lastPoint, leftPoint, GetColor());
  graphics.DrawLine(lastPoint, rightPoint, GetColor());
  graphics.DrawLine(leftPoint, rightPoint, GetColor());

  if (IsMarked()) {
    graphics.FillRectangle(Rect(leftPoint - MarkRadius,
                           leftPoint + MarkRadius), Black, Black);
    graphics.FillRectangle(Rect(rightPoint - MarkRadius,
                           rightPoint + MarkRadius), Black,Black);
  }
}

bool ArrowFigure::WriteFigureToStream(ostream& outStream) const {
  LineFigure::WriteFigureToStream(outStream);
  leftPoint.WritePointToStream(outStream);
  rightPoint.WritePointToStream(outStream);
  return ((bool) outStream);
}

bool ArrowFigure::ReadFigureFromStream(istream& inStream) {
  LineFigure::ReadFigureFromStream(inStream);
  leftPoint.ReadPointFromStream(inStream);
  rightPoint.ReadPointFromStream(inStream);
  return ((bool) inStream);
}

void ArrowFigure::WriteFigureToClipboard(InfoList& infoList)const{
  LineFigure::WriteFigureToClipboard(infoList);
  leftPoint.WritePointToClipboard(infoList);
  rightPoint.WritePointToClipboard(infoList);
}

void ArrowFigure::ReadFigureFromClipboard(InfoList& infoList) {
  LineFigure::ReadFigureFromClipboard(infoList);
  leftPoint.ReadPointFromClipboard(infoList);
  rightPoint.ReadPointFromClipboard(infoList);
}

void ArrowFigure::CalculateArrowHead() {
  int height = lastPoint.Y() - firstPoint.Y();
  int width = lastPoint.X() - firstPoint.X();

  const double Pi = 3.14159265;
  double alpha = atan2((double)height, (double)width);
  double beta = alpha + Pi;

  double leftAngle = beta - (Pi / 4);
  double rightAngle = beta + (Pi / 4);

  leftPoint.X() = lastPoint.X() +
                  (int) (ArrowLength * cos(leftAngle));
  leftPoint.Y() = lastPoint.Y() +
                  (int) (ArrowLength * sin(leftAngle));

  rightPoint.X() = lastPoint.X() +
                   (int) (ArrowLength * cos(rightAngle));
  rightPoint.Y() = lastPoint.Y() +
                   (int) (ArrowLength * sin(rightAngle));
}