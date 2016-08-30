#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"
#include "RectangleFigure.h"

RectangleFigure::RectangleFigure(const Window* windowPtr)
 :DrawFigure(windowPtr),
  rectangleMode(CreateRectangle) {
  // Empty.
}

void RectangleFigure::SetFirstPoint(Point point) {
  topLeft = point;
  bottomRight = point;
}

bool RectangleFigure::IsClick(Point mousePoint) {
  Rect topLeftRect(topLeft - MarkRadius, topLeft + MarkRadius);

  if (topLeftRect.PointInside(mousePoint)) {
    rectangleMode = TopLeftPoint;
    return true;
  }

  Point topRightPoint(bottomRight.X(), topLeft.Y());
  Rect topRectRight(topRightPoint - MarkRadius,
                    topRightPoint + MarkRadius);

  if (topRectRight.PointInside(mousePoint)) {
    rectangleMode = TopRightPoint;
    return true;
  }

  Rect bottomRightRect(bottomRight - MarkRadius,
                       bottomRight + MarkRadius);

  if (bottomRightRect.PointInside(mousePoint)) {
    rectangleMode = BottomRightPoint;
    return true;
  }

  Point bottomLeftPoint(topLeft.X(), bottomRight.Y());
  Rect bottomLeftRect(bottomLeftPoint - MarkRadius,
                      bottomLeftPoint + MarkRadius);

  if (bottomLeftRect.PointInside(mousePoint)) {
    rectangleMode = BottomLeftPoint;
    return true;
  }

  Rect areaRect(topLeft, bottomRight);
  areaRect.Normalize();

  if (IsFilled()) {
    areaRect.PointInside(mousePoint);   

    if (areaRect.PointInside(mousePoint)) {
      rectangleMode = MoveRectangle;
      return true;
    }
  }
  else {
    Rect largeAreaRect(areaRect.TopLeft() - MarkRadius,
                       areaRect.BottomRight() + MarkRadius),
         smallAreaRect(areaRect.TopLeft() + MarkRadius,
                       areaRect.BottomRight() - MarkRadius);
        
    if (largeAreaRect.PointInside(mousePoint) &&
        !smallAreaRect.PointInside(mousePoint)) {
      rectangleMode = MoveRectangle;
      return true;
    }
  }

  return false;
}

bool RectangleFigure::IsInside(Rect area) {
  return area.PointInside(topLeft) &&
         area.PointInside(bottomRight);
}

void RectangleFigure::DoubleClick(Point mousePoint) {
  if (IsClick(mousePoint)) {
    filled = !filled;
    Invalidate();
  }
}

void RectangleFigure::Modify(Size distanceSize) {
  Invalidate();

  switch (rectangleMode) {
    case TopLeftPoint:
      topLeft += distanceSize;
      break;

    case TopRightPoint:
      topLeft.Y() += distanceSize.Height();
      bottomRight.X() += distanceSize.Width();
      break;

    case CreateRectangle:
    case BottomRightPoint:
      bottomRight += distanceSize;
      break;

    case BottomLeftPoint:
      topLeft.X() += distanceSize.Width();
      bottomRight.Y() += distanceSize.Height();
      break;

    case MoveRectangle:
      Move(distanceSize);
      break;
  }

  Invalidate();
}

void RectangleFigure::Move(Size distanceSize) {
  Invalidate();
  topLeft += distanceSize;
  bottomRight += distanceSize;
  Invalidate();
}

Rect RectangleFigure::Area() const {
  Rect areaRect(topLeft, bottomRight);
  areaRect.Normalize();

  if (IsMarked()) {
    areaRect -= MarkRadius;
    areaRect += MarkRadius;
  }

  return areaRect;
}

void RectangleFigure::Draw(Graphics& graphics) const {
  if (filled) {
    graphics.FillRectangle(Rect(topLeft, bottomRight),
                           GetColor(), GetColor());
  }
  else {
    graphics.DrawRectangle(Rect(topLeft, bottomRight),
                           GetColor());
  }

  if (IsMarked()) {
    graphics.FillRectangle(Rect(topLeft - MarkRadius,
                           topLeft + MarkRadius), Black, Black);

    Point topRight(bottomRight.X(), topLeft.Y());
    graphics.FillRectangle(Rect(topRight - MarkRadius,
                           topRight + MarkRadius), Black, Black);

    graphics.FillRectangle(Rect(bottomRight - MarkRadius,
                           bottomRight + MarkRadius),Black,Black);

    Point bottomLeft(topLeft.X(), bottomRight.Y());
    graphics.FillRectangle(Rect(bottomLeft - MarkRadius,
                 bottomLeft + MarkRadius.Height()), Black, Black);
  }
}

CursorType RectangleFigure::GetCursor() const {
  switch (rectangleMode) {
    case TopLeftPoint:
    case BottomRightPoint:
      return Cursor::SizeNorthWestSouthEast;

    case TopRightPoint:
    case BottomLeftPoint:
      return Cursor::SizeNorthEastSouthWest;

    case MoveRectangle:
      return Cursor::SizeAll;

    default:
      return Cursor::Normal;
  }
}

bool RectangleFigure::WriteFigureToStream(ostream& outStream)
                                          const {
  DrawFigure::WriteFigureToStream(outStream);
  topLeft.WritePointToStream(outStream);
  bottomRight.WritePointToStream(outStream);
  outStream.write((char*) &filled, sizeof filled);
  return ((bool) outStream);
}

bool RectangleFigure::ReadFigureFromStream (istream& inStream) {
  DrawFigure::ReadFigureFromStream(inStream);
  topLeft.ReadPointFromStream(inStream);
  bottomRight.ReadPointFromStream(inStream);
  inStream.read((char*) &filled, sizeof filled);
  return ((bool)inStream);
}

void RectangleFigure::WriteFigureToClipboard(InfoList& infoList)
                                             const {
  DrawFigure::WriteFigureToClipboard(infoList);
  topLeft.WritePointToClipboard(infoList);
  bottomRight.WritePointToClipboard(infoList);
  infoList.AddValue<bool>(filled);
}

void RectangleFigure::ReadFigureFromClipboard(InfoList& infoList) {
  DrawFigure::ReadFigureFromClipboard(infoList);
  topLeft.ReadPointFromClipboard(infoList);
  bottomRight.ReadPointFromClipboard(infoList);
  infoList.GetValue<bool>(filled);
}