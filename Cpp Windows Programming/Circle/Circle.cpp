#include "..\\SmallWindows\\SmallWindows.h"
#include "Circle.h"

Circle::Circle() {
  // Empty.
}

Circle::Circle(Point center, int radius, Color color)
 :color(color),
  center(center),
  radius(radius) {
  // Empty.
}

bool Circle::WriteCircle(ostream& outStream) const {
  color.WriteColorToStream(outStream);
  center.WritePointToStream(outStream);
  outStream.write((char*) &radius, sizeof radius);
  return ((bool) outStream);
}

bool Circle::ReadCircle(istream& inStream) {
  color.ReadColorFromStream(inStream);
  center.ReadPointFromStream(inStream);
  inStream.read((char*) &radius, sizeof radius);
  return ((bool) inStream);
}

bool Circle::IsClick(Point point) const {
  int width = point.X() - center.X(),
      height = point.Y() - center.Y();
  int distance = (int) sqrt((width * width) +
                            (height * height));
  return (distance <= radius);
}

Rect Circle::Area() const {
  Point topLeft = center - radius,
        bottomRight = center + radius;
  return Rect(topLeft, bottomRight);
}

void Circle::Draw(Graphics& graphics) const {
  Point topLeft = center - radius,
        bottomRight = center + radius;
  Rect circleRect(topLeft, bottomRight);
  graphics.FillEllipse(circleRect, Black, color);
}