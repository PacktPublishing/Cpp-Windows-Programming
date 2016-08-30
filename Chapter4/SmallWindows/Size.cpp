#include "SmallWindows.h"

namespace SmallWindows {
  Size::Size()
   :width(0),
    height(0) {
    // Empty.
  }

  Size::Size(int width, int height)
   :width(width),
    height(height) {
    // Empty.
  }

  Size::Size(const Size& size)
   :width(size.width),
    height(size.height) {
    // Empty.
  }

  Size& Size::operator=(const Size& size) {
    if (this != &size) {
      width = size.width;
      height = size.height;
    }

    return *this;
  }

  Size::Size(const SIZE& size)
   :width(size.cx),
    height(size.cy) {
    // Empty.
  }

  Size& Size::operator=(const SIZE& size) {
    width = size.cx;
    height = size.cy;
    return *this;
  }

  Size::operator SIZE() const {
    SIZE size = {width, height};
    return size;
  }

  bool Size::operator==(const Size& size) const {
    return (width == size.width) && (height == size.height);
  }

  bool Size::operator!=(const Size& size) const {
    return !(*this == size);
  }

  bool Size::operator<(const Size& size) const {
    return (width < size.width) ||
           ((width == size.width) && (height < size.height));
  }

  bool Size::operator<=(const Size& size) const {
    return ((*this < size) || (*this == size));
  }

  bool Size::operator>(const Size& size) const {
    return !(*this <= size);
  }

  bool Size::operator>=(const Size& size) const {
    return !(*this < size);
  }

  Size Min(const Size& left, const Size& right) {
    return (left < right) ? left : right;
  }

  Size Max(const Size& left, const Size& right) {
    return (left > right) ? left : right;
  }

  Size Size::operator*=(double factor) {
    width = (int) (factor * width);
    height = (int) (factor * height);
    return *this;
  }

  Size operator*(const Size& size, double factor) {
    return Size((int) (size.width * factor),
                (int) (size.height * factor));
  }

  Size operator*(double factor, const Size& size) {
    return Size((int) (factor * size.width),
                (int) (factor * size.height));
  }

  Size Size::operator*=(pair<double,double> factorPair) {
    width = (int) (factorPair.first * width);
    height = (int) (factorPair.second * height);
    return *this;
  }

  Size operator*(const Size& size,
                 pair<double,double> factorPair) {
    return Size((int) (size.width * factorPair.first),
                (int) (size.height * factorPair.second));
  }

  Size operator*(pair<double,double> factorPair,
                 const Size& size) {
    return Size((int) (factorPair.first * size.width),
                (int) (factorPair.second * size.height));
  }

  Size Size::operator+=(int distance) {
    width += distance;
    height += distance;
    return *this;
  }

  Size Size::operator-=(int distance) {
    width -= distance;
    height -= distance;
    return *this;
  }

  Size operator+(const Size& size, int distance) {
    return Size(size.width + distance, size.height + distance);
  }

  Size operator-(const Size& size, int distance) {
    return Size(size.width - distance, size.height - distance);
  }

  Size Size::operator+=(const Size& size) {
    width += size.width;
    height += size.height;
    return *this;
  }

  Size Size::operator-=(const Size& size) {
    width -= size.width;
    height -= size.height;
    return *this;
  }

  Size operator+(const Size& left, const Size& right) {
    return Size(left.width + right.width, right.height + right.height);
  }

  Size operator-(const Size& left, const Size& right) {
    return Size(left.width - right.width, right.height - right.height);
  }

  bool Size::WriteSizeToStream(ostream& outStream) const {
    outStream.write((char*) &width, sizeof width);
    outStream.write((char*) &height, sizeof height);
    return ((bool) outStream);
  }

  bool Size::ReadSizeFromStream(istream& inStream) {
    inStream.read((char*) &width, sizeof width);
    inStream.read((char*) &height, sizeof height);
    return ((bool) inStream);
  }

  void Size::WriteSizeToClipboard(InfoList& infoList) const {
    infoList.AddValue<int>(width);
    infoList.AddValue<int>(height);
  }

  void Size::ReadSizeFromClipboard(InfoList& infoList) {
    infoList.GetValue<int>(width);
    infoList.GetValue<int>(height);
  }

  void Size::WriteSizeToRegistry(String key) const {
    SIZE sizeStruct = (SIZE)*this;
    Registry::WriteBuffer(key, &sizeStruct, sizeof sizeStruct);
  }

  void Size::ReadSizeFromRegistry(String key,
                                  Size defaultSize /*=ZeroSize*/){
    SIZE sizeStruct, defaultSizeStruct = (SIZE) defaultSize;
    Registry::ReadBuffer(key, &sizeStruct, sizeof sizeStruct,
                         &defaultSizeStruct);
    *this = Size(sizeStruct);
  }

  const Size ZeroSize(0, 0);
};