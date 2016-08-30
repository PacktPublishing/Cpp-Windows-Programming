#include "SmallWindows.h"

namespace SmallWindows {
  Font::Font() {
    memset(&logFont, 0, sizeof logFont);
  }

  Font::Font(String name, int size, bool italic, bool bold) {
    memset(&logFont, 0, sizeof logFont);
    wcscpy_s(logFont.lfFaceName, LF_FACESIZE, name.c_str());
    logFont.lfHeight = size;
    logFont.lfItalic = (italic ? TRUE : FALSE);
    logFont.lfWeight = (bold ? FW_BOLD : FW_NORMAL);
  }

  Font::Font(const Font& font) {
    logFont = font.LogFont();
    color = font.color;
  }

  Font& Font::operator=(const Font& font) {
    if (this != &font) {
      logFont = font.LogFont();
      color = font.color;
    }

    return *this;
  }

  bool Font::operator==(const Font& font) const {
    return (::memcmp(&logFont, &font.logFont,
      sizeof logFont) == 0) &&
      (color == font.color);
  }

  bool Font::operator!=(const Font& font) const {
    return !(*this == font);
  }

  bool Font::WriteFontToStream(ostream& outStream) const {
    outStream.write((char*) &logFont, sizeof logFont);
    color.WriteColorToStream(outStream);
    return ((bool) outStream);
  }

  bool Font::ReadFontFromStream(istream& inStream) {
    inStream.read((char*) &logFont, sizeof logFont);
    color.ReadColorFromStream(inStream);
    return ((bool) inStream);
  }

  void Font::WriteFontToClipboard(InfoList& infoList) const {
    infoList.AddValue<LOGFONT>(logFont);
    color.WriteColorToClipboard(infoList);
  }

  void Font::ReadFontFromClipboard(InfoList& infoList) {
    infoList.GetValue<LOGFONT>(logFont);
    color.ReadColorFromClipboard(infoList);
  }

  void Font::WriteFontToRegistry(String key) {
    Registry::WriteBuffer(key, &logFont, sizeof logFont);
    color.WriteColorToRegistry(key);
  }

  void Font::ReadFontFromRegistry(String key,
                         Font defaultFont /* = SystemFont */) {
    Registry::ReadBuffer(key, &logFont, sizeof logFont, &defaultFont.logFont);
    color.ReadColorFromRegistry(key);
  }

  void Font::PointsToLogical(double zoom /* = 1.0 */) {
    logFont.lfWidth =
      (int) (zoom * 2540.0 * logFont.lfWidth / 72.0);
    logFont.lfHeight =
      (int) (zoom * 2540.0 * logFont.lfHeight / 72.0);
  }

  const Font SystemFont;
//  const Font SystemFont(TEXT("Courier New"), 12);
};