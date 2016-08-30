#include "..\\SmallWindows\\SmallWindows.h"
#include "LineInfo.h"

LineInfo::LineInfo() {
  // Empty.
}

LineInfo::LineInfo(int first, int last, int top,
                   int height, int ascent)
 :first(first),
  last(last),
  top(top),
  height(height),
  ascent(ascent) {
  // Empty.
}

bool LineInfo::WriteLineInfoToStream(ostream& outStream) const {
  outStream.write((char*) &first, sizeof first);
  outStream.write((char*) &last, sizeof last);
  outStream.write((char*) &ascent, sizeof ascent);
  outStream.write((char*) &top, sizeof top);
  outStream.write((char*) &height, sizeof height);
  return ((bool) outStream);
}

bool LineInfo::ReadLineInfoFromStream(istream& inStream) {
  inStream.read((char*) &first, sizeof first);
  inStream.read((char*) &last, sizeof last);
  inStream.read((char*) &ascent, sizeof ascent);
  inStream.read((char*) &top, sizeof top);
  inStream.read((char*) &height, sizeof height);
  return ((bool) inStream);
}