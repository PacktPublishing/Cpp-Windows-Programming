#include "..\\SmallWindows\\SmallWindows.h"

namespace SmallWindows {
  PageSetupInfo::PageSetupInfo() {
    ClearPageSetupInfo();
  }

  PageSetupInfo::PageSetupInfo(const PageSetupInfo& pageSetupInfo)
   :orientation(pageSetupInfo.orientation),
    leftMargin(pageSetupInfo.leftMargin),
    topMargin(pageSetupInfo.topMargin),
    rightMargin(pageSetupInfo.rightMargin),
    bottomMargin(pageSetupInfo.bottomMargin),
    headerText(pageSetupInfo.headerText),
    footerText(pageSetupInfo.footerText),
    headerFirst(pageSetupInfo.headerFirst),
    footerFirst(pageSetupInfo.footerFirst),
    frame(pageSetupInfo.frame),
    headerFont(pageSetupInfo.headerFont),
    footerFont(pageSetupInfo.footerFont) {
     // Empty.
  }

  bool PageSetupInfo::operator==
         (const PageSetupInfo& pageSetupInfo) {
    return (orientation == pageSetupInfo.orientation) &&
           (leftMargin == pageSetupInfo.leftMargin) &&
           (topMargin == pageSetupInfo.topMargin) &&
           (rightMargin == pageSetupInfo.rightMargin) &&
           (bottomMargin == pageSetupInfo.bottomMargin) &&
           (headerText == pageSetupInfo.headerText) &&
           (footerText == pageSetupInfo.footerText) &&
           (headerFirst == pageSetupInfo.headerFirst) &&
           (footerFirst == pageSetupInfo.footerFirst) &&
           (frame == pageSetupInfo.frame) &&
           (headerFont == pageSetupInfo.headerFont) &&
           (footerFont == pageSetupInfo.footerFont);
  }

  bool PageSetupInfo::operator!=
         (const PageSetupInfo& pageSetupInfo) {
    return !(*this == pageSetupInfo);
  }

  void PageSetupInfo::ClearPageSetupInfo() {
    orientation = Portrait;
    leftMargin = 25;
    topMargin = 25;
    rightMargin = 25;
    bottomMargin = 25;
    headerText = TEXT("");
    footerText = TEXT("");
    headerFirst = true;
    footerFirst = true;
    frame = true;
    headerFont = Font(TEXT("Times New Roman"), 12, false, true);
    footerFont = Font(TEXT("Times New Roman"), 12, false);
  }

  bool PageSetupInfo::WritePageSetupInfoToStream
                      (ostream& outStream) const {
    outStream.write((char*) &orientation, sizeof orientation);
    outStream.write((char*) &leftMargin, sizeof leftMargin);
    outStream.write((char*) &topMargin, sizeof topMargin);
    outStream.write((char*) &rightMargin, sizeof rightMargin);
    outStream.write((char*) &bottomMargin, sizeof bottomMargin);
    WriteStringToStream(headerText, outStream);
    WriteStringToStream(footerText, outStream);
    outStream.write((char*) &headerFirst, sizeof headerFirst);
    outStream.write((char*) &footerFirst, sizeof footerFirst);
    outStream.write((char*) &frame, sizeof frame);
    headerFont.WriteFontToStream(outStream);
    footerFont.WriteFontToStream(outStream);
    return ((bool) outStream);
  }

  bool PageSetupInfo::ReadPageSetupInfoFromStream
                      (istream& inStream) {
    inStream.read((char*) &orientation, sizeof orientation);
    inStream.read((char*) &leftMargin, sizeof leftMargin);
    inStream.read((char*) &topMargin, sizeof topMargin);
    inStream.read((char*) &rightMargin, sizeof rightMargin);
    inStream.read((char*) &bottomMargin, sizeof bottomMargin);
    ReadStringFromStream(headerText, inStream);
    ReadStringFromStream(footerText, inStream);
    inStream.read((char*) &headerFirst, sizeof headerFirst);
    inStream.read((char*) &footerFirst, sizeof footerFirst);
    inStream.read((char*) &frame, sizeof frame);
    headerFont.ReadFontFromStream(inStream);
    footerFont.ReadFontFromStream(inStream);
    return ((bool) inStream);
  }
};

//    inStream.read((char*) &headerText, sizeof headerText);
//    inStream.read((char*) &footerText, sizeof footerText);
//    inStream.read((char*) &headerFont, sizeof headerFont);
//    inStream.read((char*) &footerFont, sizeof footerFont);

//    outStream.write((char*) &headerText, sizeof headerText);
//    outStream.write((char*) &footerText, sizeof footerText);
//    outStream.write((char*) &headerFont, sizeof headerFont);
//    outStream.write((char*) &footerFont, sizeof footerFont);
