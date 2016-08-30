#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"
#include "RectangleFigure.h"
#include "EllipseFigure.h"

EllipseFigure::EllipseFigure(const Window* windowPtr)
 :RectangleFigure(windowPtr),
  ellipseMode(CreateEllipse) {
  // Empty.
}

bool EllipseFigure::IsClick(Point mousePoint) {
  Point leftPoint(topLeft.X(), (topLeft.Y() + bottomRight.Y())/2);
  Rect leftRect(leftPoint - MarkRadius, leftPoint + MarkRadius);

  if (leftRect.PointInside(mousePoint)) {
    ellipseMode = LeftPoint;
    return true;
  }

  Point topPoint((topLeft.X() + bottomRight.X()) / 2,topLeft.Y());
  Rect topRect(topPoint - MarkRadius, topPoint + MarkRadius);

  if (topRect.PointInside(mousePoint)) {
    ellipseMode = TopPoint;
    return true;
  }

  Point rightPoint(bottomRight.X(),
                   (topLeft.Y() + bottomRight.Y()) / 2);
  Rect rightRect(rightPoint - MarkRadius,
                 rightPoint + MarkRadius);

  if (rightRect.PointInside(mousePoint)) {
    ellipseMode = RightPoint;
    return true;
  }

  Point bottomPoint((topLeft.X() + bottomRight.X()) / 2,
                    bottomRight.Y());
  Rect bottomRect(bottomPoint - MarkRadius,
                  bottomPoint + MarkRadius);

  if (bottomRect.PointInside(mousePoint)) {
    ellipseMode = BottomPoint;
    return true;
  }
 
  ellipseMode = MoveEllipse;
  Point minPoint = Min(topLeft, bottomRight),
        maxPoint = Max(topLeft, bottomRight);

  if (IsFilled()) {
    HRGN ellipseRegion =
      CreateEllipticRgn(minPoint.X(), minPoint.Y(),
                        maxPoint.X(), maxPoint.Y());
    return (PtInRegion(ellipseRegion, mousePoint.X(),
                       mousePoint.Y()) != 0);
  }
  else {
    HRGN smallRegion =
      CreateEllipticRgn(minPoint.X() + MarkRadius.Width(),
                        minPoint.Y() + MarkRadius.Height(),
                        maxPoint.X() - MarkRadius.Width(),
                        maxPoint.Y() - MarkRadius.Height());
    HRGN largeRegion =
      CreateEllipticRgn(minPoint.X() - MarkRadius.Width(),
                        minPoint.Y() - MarkRadius.Height(),
                        maxPoint.X() + MarkRadius.Width(),
                        maxPoint.Y() + MarkRadius.Height());
    return ((PtInRegion(largeRegion, mousePoint.X(),
                        mousePoint.Y()) != 0) &&
            (PtInRegion(smallRegion, mousePoint.X(),
                        mousePoint.Y()) == 0));
  }

  return false;
}

void EllipseFigure::Modify(Size distanceSize) {
  Invalidate();

  switch (ellipseMode) {
    case CreateEllipse:
      bottomRight += distanceSize;
      break;

    case LeftPoint:
      topLeft.X() += distanceSize.Width();
      break;

    case RightPoint:
      bottomRight.X() += distanceSize.Width();
      break;

    case TopPoint:
      topLeft.Y() += distanceSize.Height();
      break;

    case BottomPoint:
      bottomRight.Y() += distanceSize.Height();
      break;

    case MoveEllipse:
      Move(distanceSize);
      break;
  }

  Invalidate();
}

void EllipseFigure::Draw(Graphics& graphics) const {
  if (filled) {
    graphics.FillEllipse(Rect(topLeft, bottomRight),
                         GetColor(), GetColor());
  }
  else {
    graphics.DrawEllipse(Rect(topLeft, bottomRight), GetColor());
  }

  if (IsMarked()) {
    Point leftPoint(topLeft.X(), (topLeft.Y()+bottomRight.Y())/2);
    graphics.FillRectangle(Rect(leftPoint - MarkRadius, 
                           leftPoint + MarkRadius), Black, Black);

    Point topPoint((topLeft.X() + bottomRight.X())/2,topLeft.Y());
    graphics.FillRectangle(Rect(topPoint - MarkRadius,
                           topPoint + MarkRadius),Black, Black);

    Point rightPoint(bottomRight.X(),
                     (topLeft.Y() + bottomRight.Y()) / 2);
    graphics.FillRectangle(Rect(rightPoint - MarkRadius,
                           rightPoint + MarkRadius), Black,Black);

    Point bottomPoint((topLeft.X() + bottomRight.X()) / 2,
                      bottomRight.Y());
    graphics.FillRectangle(Rect(bottomPoint - MarkRadius,
                           bottomPoint + MarkRadius),Black,Black);
  }
}

CursorType EllipseFigure::GetCursor() const {
  switch (ellipseMode) {
    case CreateEllipse:
      return Cursor::Crosshair;

    case LeftPoint:
    case RightPoint:
      return Cursor::SizeWestEast;

    case TopPoint:
    case BottomPoint:
      return Cursor::SizeNorthSouth;

    case MoveEllipse:
      return Cursor::SizeAll;

    default:
      return Cursor::Normal;
  }
}