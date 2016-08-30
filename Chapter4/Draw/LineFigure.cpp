#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"
#include "LineFigure.h"

LineFigure::LineFigure(const Window* windowPtr)
 :DrawFigure(windowPtr),
  lineMode(CreateLine) {
  // Empty.
}

void LineFigure::SetFirstPoint(Point point) {
  firstPoint = point;
  lastPoint = point;
}

bool LineFigure::IsClick(Point mousePoint) {
  Rect firstSquare(firstPoint - MarkRadius,
                   firstPoint + MarkRadius);
  firstSquare.Normalize();

  if (firstSquare.PointInside(mousePoint)) {
    lineMode = FirstPoint;
    return true;
  }

  Rect lastSquare(lastPoint - MarkRadius, lastPoint + MarkRadius);
  lastSquare.Normalize();

  if (lastSquare.PointInside(mousePoint)) {
    lineMode = LastPoint;
    return true;
  }

  if (IsPointInLine(firstPoint, lastPoint, mousePoint)) {
    lineMode = MoveLine;
    return true;
  }

  return false;
}

bool LineFigure::IsInside(Rect rect) {
  return (rect.PointInside(firstPoint) &&
          rect.PointInside(lastPoint));
}

bool LineFigure::IsPointInLine(Point firstPoint, Point lastPoint,
                               Point point) {
  if (firstPoint.X() == lastPoint.X()) {
    Rect lineRect(firstPoint - MarkRadius,
                  lastPoint + MarkRadius);
    lineRect.Normalize();
    return lineRect.PointInside(point);
  }
  else {
    Point minPoint = Min(firstPoint, lastPoint),
          maxPoint = Max(firstPoint, lastPoint);

    if ((minPoint.X() <= point.X()) &&
        (point.X() <= maxPoint.X())) {
      int lineWidth = maxPoint.X() - minPoint.X(),
          lineHeight = maxPoint.Y() - minPoint.Y();

      int diffWidth = point.X() - minPoint.X(),
          diffHeight = point.Y() - minPoint.Y();

      double delta = fabs(diffHeight - (diffWidth *
                          ((double) lineHeight) / lineWidth));
      return (delta <= MarkRadius.Height());
    }

    return false;
  }
}

void LineFigure::Modify(Size distanceSize) {
  Invalidate();

  switch (lineMode) {
    case FirstPoint:
      firstPoint += distanceSize;
      break;

    case CreateLine:
    case LastPoint:
      lastPoint += distanceSize;
      break;

    case MoveLine:
      Move(distanceSize);
      break;
  }

  Invalidate();
}

void LineFigure::Move(Size distanceSize) {
  Invalidate();
  firstPoint += distanceSize;
  lastPoint += distanceSize;
  Invalidate();
}

void LineFigure::Draw(Graphics& graphics) const {
  graphics.DrawLine(firstPoint, lastPoint, GetColor());

  if (IsMarked()) {
    graphics.FillRectangle(Rect(firstPoint - MarkRadius,
                                firstPoint + MarkRadius), Black, Black);
    graphics.FillRectangle(Rect(lastPoint - MarkRadius,
                                lastPoint + MarkRadius), Black, Black);
  }
}

Rect LineFigure::Area() const {
  Rect lineArea(firstPoint.X(), firstPoint.Y(),
                lastPoint.X(), lastPoint.Y());
  lineArea.Normalize();

  if (IsMarked()) {
    lineArea -= MarkRadius;
    lineArea += MarkRadius;
  }

  return lineArea;
}

CursorType LineFigure::GetCursor() const {
  switch (lineMode) {
    case CreateLine:
    case FirstPoint:
    case LastPoint:
      return Cursor::Crosshair;

    case MoveLine:
      return Cursor::SizeAll;

    default:
      return Cursor::Normal;
  }
}

bool LineFigure::WriteFigureToStream(ostream& outStream) const {
  DrawFigure::WriteFigureToStream(outStream);
  firstPoint.WritePointToStream(outStream);
  lastPoint.WritePointToStream(outStream);
  return ((bool) outStream);
}

bool LineFigure::ReadFigureFromStream (istream& inStream) {
  DrawFigure::ReadFigureFromStream(inStream);
  firstPoint.ReadPointFromStream(inStream);
  lastPoint.ReadPointFromStream(inStream);
  return ((bool) inStream);
}

void LineFigure::WriteFigureToClipboard(InfoList& infoList) const{
  DrawFigure::WriteFigureToClipboard(infoList);
  firstPoint.WritePointToClipboard(infoList);
  lastPoint.WritePointToClipboard(infoList);
}

void LineFigure::ReadFigureFromClipboard(InfoList& infoList) {
  DrawFigure::ReadFigureFromClipboard(infoList);
  firstPoint.ReadPointFromClipboard(infoList);
  lastPoint.ReadPointFromClipboard(infoList);
}