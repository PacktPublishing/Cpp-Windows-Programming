#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"

CharInfo::CharInfo(Paragraph* paragraphPtr /* = nullptr */,
                   TCHAR tChar /* = TEXT('\0') */,
                   Font font/* = SystemFont */, 
                   Rect rect /* = ZeroRect */)
 :lineInfoPtr(nullptr),
  paragraphPtr(paragraphPtr),
  tChar(tChar),
  charFont(font),
  charRect(rect) {
  // Empty.
}

CharInfo::CharInfo(const CharInfo& charInfo)
 :lineInfoPtr(charInfo.lineInfoPtr),
  paragraphPtr(charInfo.paragraphPtr),
  tChar(charInfo.tChar),
  charFont(charInfo.charFont),
  charRect(charInfo.charRect) {
  // Empty.
}

CharInfo& CharInfo::operator=(const CharInfo& charInfo) {
  if (this != &charInfo) {
    lineInfoPtr = charInfo.lineInfoPtr;
    paragraphPtr = charInfo.paragraphPtr;
    tChar = charInfo.tChar;
    charFont = charInfo.charFont;
    charRect = charInfo.charRect;
  }

  return *this;
}

bool CharInfo::WriteCharInfoToStream(ostream& outStream) const {
  outStream.write((char*) &tChar, sizeof tChar);
  charFont.WriteFontToStream(outStream);
  charRect.WriteRectToStream(outStream);
  return ((bool) outStream);
}

bool CharInfo::ReadCharInfoFromStream(istream& inStream) {
  inStream.read((char*) &tChar, sizeof tChar);
  charFont.ReadFontFromStream(inStream);
  charRect.ReadRectFromStream(inStream);
  return ((bool) inStream);
}

void CharInfo::WriteCharInfoToClipboard(InfoList& infoList) const{
  infoList.AddValue<TCHAR>(tChar);
  charFont.WriteFontToClipboard(infoList);
}

void CharInfo::ReadCharInfoFromClipboard(InfoList& infoList) {
  infoList.GetValue<TCHAR>(tChar);
  charFont.ReadFontFromClipboard(infoList);
}