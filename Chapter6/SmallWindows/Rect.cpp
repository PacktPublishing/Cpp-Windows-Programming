#include "SmallWindows.h"

namespace SmallWindows {
  Rect::Rect()
   :left(0), top(0), right(0), bottom(0) {
    // Empty.
  }

  Rect::Rect(int left, int top, int right, int bottom)
   :left(left),
    top(top),
    right(right),
    bottom(bottom) {
    // Empty.
  }

  Rect::Rect(const Rect& rect)
   :left(rect.left),
    top(rect.top),
    right(rect.right),
    bottom(rect.bottom) {
    // Empty.
  }

  Rect& Rect::operator=(const Rect& rect) {
    if (this != &rect) {
      left = rect.left;
      top = rect.top;
      right = rect.right;
      bottom = rect.bottom;
    }

    return *this;
  }

  Rect::Rect(Point topLeft, Point bottomRight)
   :left(topLeft.X()),
    top(topLeft.Y()),
    right(bottomRight.X()),
    bottom(bottomRight.Y()) {
    // Empty.
  }

  Rect::Rect(Point topLeft, Size size)
   :left(topLeft.X()),
    top(topLeft.Y()),
    right(topLeft.X() + size.Width()),
    bottom(topLeft.Y() + size.Height()) {
    // Empty.
  }

  Rect::Rect(const RECT& rect)
   :left(rect.left),
    top(rect.top),
    right(rect.right),
    bottom(rect.bottom) {
    // Empty.
  }

  Rect& Rect::operator=(const RECT& rect) {
    left = rect.left;
    top = rect.top;
    right = rect.right;
    bottom = rect.bottom;
    return *this;
  }

  Rect::operator RECT() const {
    RECT rect = {left, top, right, bottom};
    return rect;
  }

  bool Rect::operator==(const Rect& rect) const {
    return (left == rect.left) && (top == rect.top) &&
           (right == rect.right) && (bottom == rect.bottom);
  }

  bool Rect::operator!=(const Rect& rect) const {
    return !(*this == rect);
  }

  bool Rect::operator<(const Rect& rect) const {
    return (TopLeft() < rect.TopLeft()) ||
           ((TopLeft() == rect.TopLeft()) &&
            (BottomRight() < rect.BottomRight()));
  }

  bool Rect::operator<=(const Rect& rect) const {
    return ((*this < rect) || (*this == rect));
  }

  bool Rect::operator>(const Rect& rect) const {
    return !(*this <= rect);
  }

  bool Rect::operator>=(const Rect& rect) const {
    return !(*this < rect);
  }

  Rect& Rect::operator*=(double factor) {
    left = (int) (factor * left);
    top = (int) (factor * top);
    right = (int) (factor * right);
    bottom = (int) (factor * bottom);
    return *this;
  }

  Rect operator*(const Rect& rect, double factor) {
    return Rect(rect.TopLeft() * factor,
                rect.BottomRight() * factor);
  }

  Rect operator*(double factor, const Rect& rect) {
    return Rect(factor * rect.TopLeft(),
                factor * rect.BottomRight());
  }

  Rect& Rect::operator*=(pair<double,double> factorPair) {
    left = (int) (factorPair.first * left);
    top = (int) (factorPair.second * top);
    right = (int) (factorPair.first * right);
    bottom = (int) (factorPair.second * bottom);
    return *this;
  }

  Rect operator*(const Rect& rect, pair<double,double> factorPair) {
    return Rect(rect.TopLeft() * factorPair, rect.BottomRight() * factorPair);
  }

  Rect operator*(pair<double,double> factorPair, const Rect& rect) {
    return Rect(factorPair * rect.TopLeft(), factorPair * rect.BottomRight());
  }

  Rect& Rect::operator+=(const Size& size) {
    right += size.Width();
    bottom += size.Height();
    return *this;
  }

  Rect& Rect::operator-=(const Size& size) {
    left -= size.Width();
    top -= size.Height();
    return *this;
  }

  Rect operator+(const Rect& rect, const Size& size) {
    return Rect(rect.left + size.Width(),
                rect.top + size.Height(),
                rect.right + size.Width(),
                rect.bottom + size.Height());
  }

  Rect operator-(const Rect& rect, const Size& size) {
    return Rect(rect.left - size.Width(),
                rect.top - size.Height(),
                rect.right - size.Width(),
                rect.bottom - size.Height());
  }

  Rect& Rect::operator+=(const Point& point) {
    left += point.X();
    top += point.Y();
    right += point.X();
    bottom += point.Y();
    return *this;
  }

  Rect& Rect::operator-=(const Point& point) {
    left -= point.X();
    top -= point.Y();
    right -= point.X();
    bottom -= point.Y();
    return *this;
  }

  Rect operator+(const Rect& rect, const Point& point) {
    return Rect(rect.left + point.X(), rect.top + point.Y(),
                rect.right + point.X(), rect.bottom + point.Y());
  }

  Rect operator+(const Point& point, const Rect& rect) {
    return Rect(point.X() + rect.left, point.Y() + rect.top,
                point.X() + rect.right, point.Y() + rect.bottom);
  }

  Rect operator-(const Rect& rect, const Point& point) {
    return Rect(rect.left - point.X(), rect.top - point.Y(),
                rect.right - point.X(), rect.bottom - point.Y());
  }

  void Rect::Clear() {
    left = top = right = bottom = 0;
  }

  void Rect::Normalize() {
    int minX = min(left, right), minY = min(top, bottom),
        maxX = max(left, right), maxY = max(top, bottom);

    left = minX;
    top = minY;
    right = maxX;
    bottom = maxY;
  }

  bool Rect::PointInside(Point point) const {
    return ((left <= point.X()) && (point.X() <= right) &&
            (top <= point.Y()) && (point.Y() <= bottom));
  }

  bool Rect::WriteRectToStream(ostream& outStream) const {
    outStream.write((char*) &left, sizeof left);
    outStream.write((char*) &top, sizeof top);
    outStream.write((char*) &right, sizeof right);
    outStream.write((char*) &bottom, sizeof bottom);
    return ((bool) outStream);
  }

  bool Rect::ReadRectFromStream(istream& inStream) {
    inStream.read((char*) &left, sizeof left);
    inStream.read((char*) &top, sizeof top);
    inStream.read((char*) &right, sizeof right);
    inStream.read((char*) &bottom, sizeof bottom);
    return ((bool) inStream);
  }

  void Rect::WriteRectToClipboard(InfoList& infoList) const {
    infoList.AddValue<int>(left);
    infoList.AddValue<int>(top);
    infoList.AddValue<int>(right);
    infoList.AddValue<int>(bottom);
  }

  void Rect::ReadRectFromClipboard(InfoList& infoList) {
    infoList.GetValue<int>(left);
    infoList.GetValue<int>(top);
    infoList.GetValue<int>(right);
    infoList.GetValue<int>(bottom);
  }

  void Rect::WriteRectToRegistry(String key) const {
    RECT pointStruct = (RECT)*this;
    Registry::WriteBuffer(key, &pointStruct, sizeof pointStruct);
  }

  void Rect::ReadRectFromRegistry(String key,
                             Rect defaultRect /* = ZeroRect */) {
    RECT rectStruct, defaultRectStruct = (RECT) defaultRect;
    Registry::ReadBuffer(key, &rectStruct, sizeof rectStruct,
                         &defaultRectStruct);
    *this = Rect(rectStruct);
  }

  const Rect ZeroRect(0, 0, 0, 0);
};