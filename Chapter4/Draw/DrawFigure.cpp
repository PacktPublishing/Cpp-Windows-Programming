#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"

const Size DrawFigure::MarkRadius(100, 100);

DrawFigure::DrawFigure(const Window* windowPtr)
 :windowPtr(windowPtr) {
  // Empty.
}

void DrawFigure::Mark(bool mark) {
  if (!marked && mark) {
    marked = true;
    Invalidate();
  }
  else if (marked && !mark) {
    Invalidate();
    marked = false;
  }
}

void DrawFigure::SetColor(Color c) {
  if (color != c) {
    color = c;
    Invalidate();
  }
}

bool DrawFigure::WriteFigureToStream(ostream& outStream) const {
  color.WriteColorToStream(outStream);
  return ((bool) outStream);
}

bool DrawFigure::ReadFigureFromStream(istream& inStream) {
  color.ReadColorFromStream(inStream);
  return ((bool) inStream);
}

void DrawFigure::WriteFigureToClipboard(InfoList& infoList) const {
  color.WriteColorToClipboard(infoList);
}

void DrawFigure::ReadFigureFromClipboard(InfoList& infoList) {
  color.ReadColorFromClipboard(infoList);
}