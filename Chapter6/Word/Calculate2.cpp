#include "..\\SmallWindows\\SmallWindows.h"
#include "Position.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

void Paragraph::Calculate(bool invalidate /* = true */, Font *fontPtr /* = NULL */,
                          int fontIndex /* = -1 */) {
  if (m_charList.empty()) {
    GenerateEmptyList(invalidate, fontPtr);
  }
  else {
    vector<CharInfo> oldCharList = m_charList;
    vector<Size> sizeList;
    vector<int> ascentList;

    GenerateSizeAndAscentList(sizeList, ascentList, fontPtr, fontIndex);
    GenerateLineList(sizeList, ascentList);

    m_height = 0;
    for each (LineInfo lineInfo in m_lineList) {
      GenerateLineRectList(lineInfo, sizeList, ascentList);
      m_height += lineInfo.m_height;
    }

    if (invalidate) {
      return GenerateRepaintSet(oldCharList, *repaintListPtr);
    }
  }
}

void Paragraph::GenerateEmptyList(set<Rect>* repaintListPtr, Font *fontPtr) {
  m_emptyFont = (fontPtr != NULL) ? (*fontPtr) : m_emptyFont;
  int averageWidth=m_wordDocumentPtr->GetCharacterAverageWidth(m_emptyFont);
      m_height = m_wordDocumentPtr->GetCharacterHeight(m_emptyFont);
  Size emptySize(averageWidth, m_height);

  switch (m_alignment) {
    case Left:
    case Justified:
      m_emptyRect = Rect(Point(0, 0), emptySize);
      break;

    case Center:
      m_emptyRect = Rect(Point((PageWidth() - averageWidth)/2, 0), emptySize);
      break;

    case Right:
      m_emptyRect = Rect(Point(PageWidth() - averageWidth, 0), emptySize);
      break;
  }

  if (repaintListPtr != NULL) {
    Point topLeft(0, m_yStartPos);
    Rect lineRect(0, 0, PageWidth(), m_height);
    repaintListPtr->insert(lineRect + topLeft);
  }
}

void Paragraph::GenerateSizeAndAscentList(vector<Size>& sizeList,
  vector<int>& ascentList, Font *fontPtr, int fontIndex){
  int index = 0;

  for each (CharInfo charInfo in m_charList) {
    int width = m_wordDocumentPtr->GetCharacterWidth(charInfo.m_font,
      charInfo.m_tChar),
      height = m_wordDocumentPtr->GetCharacterHeight(charInfo.m_font),
      ascent = m_wordDocumentPtr->GetCharacterAscent(charInfo.m_font);

    if (index++ == fontIndex) {
      ascent = max(ascent, m_wordDocumentPtr->GetCharacterAscent(*fontPtr));
      height = max(height, m_wordDocumentPtr->GetCharacterHeight(*fontPtr));
    }

    sizeList.push_back(Size(width, height));
    ascentList.push_back(ascent);
  }
}

void Paragraph::GenerateLineList(vector<Size>& sizeList,
                                 vector<int>& ascentList) {
  int lineHeight = 0, lineAscent = 0, lineWidth = 0,
      spaceLineHeight = 0, spaceLineAscent = 0, start = 0, space = -1;

  m_height = 0;
  m_lineList.clear();

  int size = m_charList.size();
  for (int index = 0; index < size;) {
    CharInfo charInfo = m_charList[index];

    lineAscent = max(lineAscent, ascentList[index]);
    lineHeight = max(lineHeight, sizeList[index].CY);
    lineWidth += sizeList[index].CX;

    if (charInfo.m_tChar == TEXT(' ')) {
      space = index;

      spaceLineAscent = max(spaceLineAscent, lineAscent);
      spaceLineHeight = max(spaceLineHeight, lineHeight);
      
      lineAscent = 0;
      lineHeight = 0;
    }

    if (lineWidth > PageWidth()) {
      LineInfo lineInfo;

      if (space != -1) {
        lineInfo.m_first = start;
        lineInfo.m_last = space - 1;
        lineInfo.m_ascent = spaceLineAscent;
        lineInfo.m_height = spaceLineHeight;
        start = space + 1;
      }
      else {
        if (index > start) {
          lineInfo.m_first = start;
          lineInfo.m_last = index - 1;
          start = index;
        }
        else {
          lineInfo.m_first = index;
          lineInfo.m_last = index;
          start = index + 1;
        }

        lineInfo.m_ascent = lineAscent;
        lineInfo.m_height = lineHeight;
      }

      m_lineList.push_back(lineInfo);
      lineWidth = 0;

      lineAscent = 0;
      lineHeight = 0;

      index = start;
      space = -1;
    }
    else {
      ++index;
    }
  }

  if (start < ((int) m_charList.size())) {
    CharInfo startInfo = m_charList[start];
    LineInfo lastInfo(start, m_charList.size() - 1,
                      max(lineHeight, spaceLineHeight),
                      max(lineAscent, spaceLineAscent));
    m_lineList.push_back(lastInfo);
  }
}

void Paragraph::GenerateLineRectList(LineInfo lineInfo, vector<Size> sizeList,
                                     vector<int> ascentList) {
  int spaces = 0, lineWidth = 0;

  for (int index = lineInfo.m_first; index <= lineInfo.m_last; ++index) {
    CharInfo charInfo = m_charList[index];

    if ((m_alignment == Justified) && (charInfo.m_tChar == TEXT(' '))) {
      ++spaces;
    }
    else {
      lineWidth += sizeList[index].CX;
    }
  }

  int leftPos, spaceWidth;

  switch (m_alignment) {
    case Left:
      leftPos = 0;
      break;

    case Center:
      leftPos = (PageWidth() - lineWidth) / 2;
      break;

    case Right:
      leftPos = PageWidth() - lineWidth;
      break;

    case Justified:
      leftPos = 0;

      if (spaces > 0) {
        spaceWidth = (PageWidth() - lineWidth) / spaces;
      }
      break;
  }

  for (int index = lineInfo.m_first; index <= lineInfo.m_last; ++index) {
    Size charSize = sizeList[index];
    int ascent = ascentList[index], charWidth;

    if ((m_alignment == Justified) &&
        (m_charList[index].m_tChar == TEXT(' '))) {
      charWidth = spaceWidth;
    }
    else {
      charWidth = charSize.CX;
    }

    int yTopPos = m_height + lineInfo.m_ascent - ascent;
    m_charList[index].m_charRect = Rect(leftPos, yTopPos, leftPos + charWidth,
                                        yTopPos + charSize.CY);
    leftPos += charWidth;
  }

  if (((lineInfo.m_last + 1) < ((int) m_charList.size())) &&
      (m_charList[lineInfo.m_last + 1].m_tChar == TEXT(' '))) {
    int ascent = ascentList[lineInfo.m_last + 1];
    Size charSize = sizeList[lineInfo.m_last + 1];

    int yTopPos = m_height + lineInfo.m_ascent - ascent;
    m_charList[lineInfo.m_last + 1].m_charRect =
      Rect(leftPos, yTopPos, leftPos + charSize.CX, yTopPos + charSize.CY);
  }
}

void Paragraph::GenerateRepaintSet(vector<CharInfo>& oldCharList,
                                   set<Rect>& repaintSet) {
  Point topLeft(0, m_yStartPos);

  int size = m_charList.size();
  for (int index = 0; index < size; ++index) {
    CharInfo oldInfo = oldCharList[index],
             newInfo = m_charList[index];
    
    if (oldInfo.m_charRect != newInfo.m_charRect) {
      repaintSet.insert(oldInfo.m_charRect + topLeft);
      repaintSet.insert(newInfo.m_charRect + topLeft);
    }
  }

  int currHeight = 0;
  for each (LineInfo lineInfo in m_lineList) {
    CharInfo firstChar = m_charList[lineInfo.m_first],
             lastChar = m_charList[lineInfo.m_last];

    if (firstChar.m_charRect.Left > 0) {
      Rect leftLine(0, currHeight, firstChar.m_charRect.Left,
                    currHeight + lineInfo.m_height);
      repaintSet.insert(leftLine + topLeft);
    }

    if (lastChar.m_charRect.Left < PageWidth()) {
      Rect rightLine(lastChar.m_charRect.Left, currHeight,
                     PageWidth(), currHeight + lineInfo.m_height);
      repaintSet.insert(rightLine + topLeft);
    }

    currHeight += lineInfo.m_height;
  }
}
