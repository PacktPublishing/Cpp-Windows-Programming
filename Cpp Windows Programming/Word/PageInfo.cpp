#if 0
#include "..\\SmallWindows\\SmallWindows.h"
#include "PageInfo.h"

PageInfo::PageInfo() {
  // Empty.
}

PageInfo::PageInfo(int firstParagraph, int lastParagraph, int height)
 :m_firstParagraph(firstParagraph),
  m_lastParagraph(lastParagraph),
  m_height(height) {
  // Empty.
}

bool PageInfo::WritePageInfoToStream(ostream& outStream) const {
  outStream.write((char*) &m_firstParagraph, sizeof m_firstParagraph);
  outStream.write((char*) &m_lastParagraph, sizeof m_lastParagraph);
  outStream.write((char*) &m_height, sizeof m_height);
  return ((bool) outStream);
}

bool PageInfo::ReadPageInfoFromStream(istream& inStream) {
  inStream.read((char*) &m_firstParagraph, sizeof m_firstParagraph);
  inStream.read((char*) &m_lastParagraph, sizeof m_lastParagraph);
  inStream.read((char*) &m_height, sizeof m_height);
  return ((bool) inStream);
}
#endif
