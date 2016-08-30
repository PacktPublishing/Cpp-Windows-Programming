#include "SmallWindows.h"

namespace SmallWindows {
  Point::Point()
   :x(0), y(0) {
    // Empty.
  }

  Point::Point(int x, int y)
   :x(x), y(y) {
    // Empty.
  }

  Point::Point(const Point& point)
   :x(point.x),
    y(point.y) {
    // Empty.
  }

  Point& Point::operator=(const Point& point) {
    if (this != &point) {
      x = point.x;
      y = point.y;
    }

    return *this;
  }

  Point::Point(const POINT& point)
   :x(point.x),
    y(point.y) {
    // Empty.
  }

  Point& Point::operator=(const POINT& point) {
    x = point.x;
    y = point.y;
    return *this;
  }

  Point::operator POINT() const {
    POINT point = {x, y};
    return point;
  }

  bool Point::operator==(const Point& point) const {
    return ((x == point.x) && (y == point.y));
  }

  bool Point::operator!=(const Point& point) const {
    return !(*this == point);
  }

  bool Point::operator<(const Point& point) const {
    return (x < point.x) || ((x == point.x) && (y < point.y));
  }

  bool Point::operator<=(const Point& point) const {
    return ((*this < point) || (*this == point));
  }

  bool Point::operator>(const Point& point) const {
    return !(*this <= point);
  }

  bool Point::operator>=(const Point& point) const {
    return !(*this < point);
  }

  Point Min(const Point& left, const Point& right) {
    return (left < right) ? left : right;
  }

  Point Max(const Point& left, const Point& right) {
    return (left > right) ? left : right;
  }

  Point& Point::operator*=(double factor) {
    x = (int)(factor * x);
    y = (int)(factor * y);
    return *this;
  }

  Point operator*(const Point& point, double factor) {
    return Point((int) (point.x * factor),
                 (int) (point.y * factor));
  }

  Point operator*(double factor, const Point& point) {
    return Point((int) (factor * point.x),
                 (int) (factor * point.y));
  }

  Point& Point::operator*=(pair<double,double> factorPair) {
    x = (int) (factorPair.first * x);
    y = (int) (factorPair.second * y);
    return *this;
  }

  Point operator*(const Point& point,
                  pair<double,double> factorPair) {
    return Point((int) (point.x * factorPair.first),
                 (int) (point.y * factorPair.second));
  }

  Point operator*(pair<double,double> factorPair,
                  const Point& point) {
    return Point((int) (factorPair.first * point.x),
                 (int) (factorPair.second * point.y));
  }

  Point& Point::operator+=(const int distance) {
    x += distance;
    y += distance;
    return *this;
  }

  Point& Point::operator-=(const int distance) {
    x -= distance;
    y -= distance;
    return *this;
  }

  Point& Point::operator+=(const Size& size) {
    x += size.Width();
    y += size.Height();
    return *this;
  }

  Point& Point::operator-=(const Size& size) {
    x -= size.Width();
    y -= size.Height();
    return *this;
  }

  Point& Point::operator+=(const Point& point) {
    x += point.x;
    y += point.y;
    return *this;
  }

  Point& Point::operator-=(const Point& point) {
    x -= point.x;
    y -= point.y;
    return *this;
  }

  Point operator+(const Point& left, int distance) {
    return Point(left.x + distance, left.y + distance);
  }

  Point operator-(const Point& left, int distance) {
    return Point(left.x - distance, left.y - distance);
  }

  Point operator+(const Point& point, const Size& size) {
    return Point(point.x + size.Width(), point.y + size.Height());
  }

  Point operator-(const Point& point, const Size& size) {
    return Point(point.x - size.Width(), point.y - size.Height());
  }

  Point operator+(const Point& left, const Point& right) {
    return Point(left.x + right.x, left.y + right.y);
  }

  Size operator-(const Point& left, const Point& right) {
    return Size(left.x - right.x, left.y - right.y);
  }

  bool Point::WritePointToStream(ostream& outStream) const {
    outStream.write((char*) &x, sizeof x);
    outStream.write((char*) &y, sizeof y);
    return ((bool) outStream);
  }

  bool Point::ReadPointFromStream(istream& inStream) {
    inStream.read((char*) &x, sizeof x);
    inStream.read((char*) &y, sizeof y);
    return ((bool) inStream);
  }

  void Point::WritePointToClipboard(InfoList& infoList) const {
    infoList.AddValue<int>(x);
    infoList.AddValue<int>(y);
  }

  void Point::ReadPointFromClipboard(InfoList& infoList) {
    infoList.GetValue<int>(x);
    infoList.GetValue<int>(y);
  }

  void Point::WritePointToRegistry(String key) const {
    POINT pointStruct = (POINT) *this;
    Registry::WriteBuffer(key, &pointStruct, sizeof pointStruct);
  }

  void Point::ReadPointFromRegistry(String key,
                           Point defaultPoint /* = ZeroPoint */) {
    POINT pointStruct, defaultPointStruct = (POINT) defaultPoint;
    Registry::ReadBuffer(key, &pointStruct, sizeof pointStruct,
                         &defaultPointStruct);
    *this = Point(pointStruct);
  }

  const Point ZeroPoint(0, 0);
};