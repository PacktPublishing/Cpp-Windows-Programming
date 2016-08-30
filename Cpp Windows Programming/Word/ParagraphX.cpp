#if 0
#include "..\\SmallWindows\\SmallWindows.h"
#include "Position.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "PageInfo.h"
#include "Paragraph.h"
#include "ParagraphX.h"
#include "WordDocument.h"

ParagraphX::ParagraphX(WordDocument* wordDocumentPtr, String text
                     /* = ZeroString */, Alignment alignment /* = Left */,
                     Font font /* = SystemFont */, Color color /* = SystemColor*/)
 :m_wordDocumentPtr(wordDocumentPtr),
  m_yStartPos(-1),
  m_height(-1),
  m_alignment(alignment),
  m_pageBreak(false) {

  for each (TCHAR tChar in text) {
    m_charList.push_back(CharInfo(tChar, font, color));
  }

  if (m_charList.empty()) {
    m_emptyFont = font;
    m_emptyColor = color;
  }

  CalculateParagraph();
}

int ParagraphX::PageWidth() const {
  return m_wordDocumentPtr->PageWidth();
}

int ParagraphX::PageHeight() const {
  return m_wordDocumentPtr->PageHeight();
}

String ParagraphX::ExtractText() const {
  String text;

  for each (CharInfo charInfo in m_charList) {
    text += charInfo.m_tChar;
  }

  return text;
}

bool ParagraphX::WriteParagraphToStream(ostream& outStream) const {
  outStream.write((char*) &m_yStartPos, sizeof m_yStartPos);
  outStream.write((char*) &m_height, sizeof m_height);
  outStream.write((char*) &m_alignment, sizeof m_alignment);
  outStream.write((char*) &m_pageBreak, sizeof m_pageBreak);
  m_emptyRect.WriteRectToStream(outStream);

  { int charListSize = m_charList.size();
    outStream.write((char*) &charListSize, sizeof charListSize);

    for each (CharInfo charInfo in m_charList) {
      charInfo.WriteCharInfoToStream(outStream);
    }
  }

  { int lineListSize = m_lineList.size();
    outStream.write((char*) &lineListSize, sizeof lineListSize);

    for each (LineInfo* lineInfoPtr in m_lineList) {
      lineInfoPtr->WriteLineInfoToStream(outStream);
    }
  }

  return ((bool) outStream);
}

bool ParagraphX::ReadParagraphFromStream(istream& inStream) {
  inStream.read((char*) &m_yStartPos, sizeof m_yStartPos);
  inStream.read((char*) &m_height, sizeof m_height);
  inStream.read((char*) &m_alignment, sizeof m_alignment);
  inStream.read((char*) &m_pageBreak, sizeof m_pageBreak);
  m_emptyRect.ReadRectFromStream(inStream);

  { int charListSize;
    inStream.read((char*) &charListSize, sizeof charListSize);

    for (int index = 0; index < charListSize; ++index) {
      CharInfo charInfo;
      charInfo.ReadCharInfoFromStream(inStream);
      m_charList.push_back(charInfo);
    }
  }

  { int lineListSize;
    inStream.read((char*) &lineListSize, sizeof lineListSize);

    for (int index = 0; index < lineListSize; ++index) {
      LineInfo* lineInfoPtr;
      lineInfoPtr->ReadLineInfoFromStream(inStream);
      m_lineList.push_back(lineInfoPtr);
    }
  }

  return ((bool) inStream);
}

void ParagraphX::WriteParagraphToCharList(list<char>& charList) const {
  int listSize = m_charList.size();
  AddToCharList(charList, &listSize, sizeof listSize);

  for each (CharInfo charInfo in m_charList) {
    charInfo.WriteCharInfoToCharList(charList);
  }
}

void ParagraphX::ReadParagraphFromBuffer(char** bufferPtr) {
  int listSize;
  MemoryCopy(&listSize, bufferPtr, sizeof listSize);

  for (int index = 0; index < listSize; ++index) {
    CharInfo charInfo;
    charInfo.ReadCharInfoFromBuffer(bufferPtr);
    m_charList.push_back(charInfo);
  }
}

void ParagraphX::Draw(Graphics& graphics, bool marked,
                     int firstMark /* = 0 */, int lastMark /* = -1 */) {
  Point topLeft(0, m_yStartPos);

  if (marked && (lastMark == -1)) {
    lastMark = m_charList.size();
  }

  if (!m_charList.empty()) {
    int size = m_charList.size();

    for (int charIndex = 0; charIndex < size; ++charIndex) {
      CharInfo charInfo = m_charList[charIndex];

      Color textColor = charInfo.m_color,
            backColor = charInfo.m_color.Inverse();

      if (marked && (charIndex >= firstMark) && (charIndex < lastMark)) {
        textColor = textColor.Inverse();
        backColor = backColor.Inverse();
      }

      TCHAR text[] = {charInfo.Char, ZeroChar};
      graphics.DrawText(charInfo.m_charRect + topLeft, text,
                        charInfo.m_font, textColor, backColor);
    }
  }
}

/*  else {
    int width = m_wordDocumentPtr->GetCharacterAverageWidth(m_emptyFont),
        height = m_wordDocumentPtr->GetCharacterHeight(m_emptyFont);
    Rect drawRect(Point(0, m_yStartPos), Size(width, height));
    graphics.FillRectangle(Rect(topLeft, Size(width, height)),
                           m_emptyColor.Inverse(), m_emptyColor);
  }*/

void ParagraphX::AddChar(int charIndex, TCHAR newChar,
                        Font newFont, Color newColor) {
  if (newFont == SystemFont) {
    if (m_charList.empty()) {
      newFont = m_emptyFont;
      newColor = m_emptyColor;
    }
    else if (charIndex == m_charList.size()) {
      newFont = m_charList[charIndex - 1].m_font;
      newColor = m_charList[charIndex - 1].m_color;
    }
    else {
      newFont = m_charList[charIndex].m_font;
      newColor = m_charList[charIndex].m_color;
    }
  }

  CharInfo charInfo(newChar, newFont, newColor, ZeroRect);

  if (charIndex == m_charList.size()) {
    m_charList.push_back(charInfo);
  }
  else {
    switch (m_wordDocumentPtr->CheckKeyboardState()) {
      case InsertKeyboard:
        m_charList.insert(charIndex, charInfo);
        break;

      case OverwriteKeyboard:
        m_charList[charIndex] = charInfo;
        break;
    }
  }
}

void ParagraphX::InvalidateBlock(int firstIndex, int lastIndex /* = -1 */) {
  int listSize = m_charList.size();

  if (firstIndex == -1) {
    firstIndex = 0;
  }

  if ((lastIndex == -1) || (lastIndex >= listSize)) {
    lastIndex = listSize;
  }

  Point topLeft(0, m_yStartPos);
  for (int charIndex = firstIndex; charIndex < lastIndex; ++charIndex) {
    m_wordDocumentPtr->Invalidate(m_charList[charIndex].m_charRect + topLeft);
  }
}

void ParagraphX::OnDelete(int firstIndex /* = 0 */, int lastIndex /* = -1 */) {
  if (!m_charList.empty()) {
    lastIndex = (lastIndex == -1) ? m_charList.size() : lastIndex;

    if ((firstIndex == 0) && (lastIndex == m_charList.size())) {
      m_emptyFont = m_charList[0].m_font;
      m_emptyColor = m_charList[0].m_color;
      m_charList.clear();
    }
    else {
      m_charList.erase(firstIndex, lastIndex);
    }
  }
}

Font ParagraphX::GetFont(int charIndex) {
  if (m_charList.empty()) {
    return m_emptyFont;
  }
  else if (charIndex == m_charList.size()) {
    return m_charList[charIndex - 1].m_font;
  }
  else {
    return m_charList[charIndex].m_font;
  }
}

void ParagraphX::SetFont(Font font, int firstIndex, int lastIndex) {
  if (!m_charList.empty()) {
    lastIndex = (lastIndex == -1) ? (m_charList.size() - 1) : lastIndex;

    for (int charIndex = firstIndex; charIndex <= lastIndex; ++charIndex) {
      m_charList[charIndex].m_font = font;
    }
  }
  else {
    m_emptyFont = font;
  }
}

Color ParagraphX::GetColor(int charIndex) {
  if (m_charList.empty()) {
    return m_emptyColor;
  }
  else if (charIndex == m_charList.size()) {
    return m_charList[charIndex - 1].m_color;
  }
  else {
    return m_charList[charIndex].m_color;
  }
}

void ParagraphX::SetColor(Color color, int firstIndex, int lastIndex) {
  if (!m_charList.empty()) {
    lastIndex = (lastIndex == -1) ? (m_charList.size() - 1) : lastIndex;

    for (int charIndex = firstIndex; charIndex <= lastIndex; ++charIndex) {
      m_charList[charIndex].m_color = color;
    }
  }
  else {
    m_emptyColor = color;
  }
}

ParagraphX* ParagraphX::Extract(int firstIndex, int lastIndex) {
  lastIndex = (lastIndex == -1) ? (m_charList.size() - 1) : lastIndex;
  ParagraphX* extractPtr = new ParagraphX(m_wordDocumentPtr);

  for (int charIndex = firstIndex; charIndex <= lastIndex; ++charIndex) {
    extractPtr->m_charList.push_back(m_charList[charIndex]);
  }

  return extractPtr;
}

void ParagraphX::InsertParagraph(int charIndex, ParagraphX* insertPtr) {
  m_charList.insert(charIndex, insertPtr->m_charList);
}

void ParagraphX::AppendParagraph(ParagraphX* appendPtr) {
  InsertParagraph(m_charList.size(), appendPtr);
}

void ParagraphX::InsertText(int charIndex, String text) {
  Font newFont = GetFont(charIndex);
  Color newColor = GetColor(charIndex);

  for each (char c in text) {
    CharInfo charInfo(c, newFont, newColor);
    m_charList.insert(charIndex++, charInfo);
  }
}

void ParagraphX::AppendText(String text) {
  InsertText(m_charList.size() - 1, text);
}

ParagraphX* ParagraphX::Split(int charIndex) {
  ParagraphX* splitPtr = new ParagraphX(m_wordDocumentPtr);
  splitPtr->SetAlignment(m_alignment);

  if (m_charList.empty()) {
    splitPtr->m_emptyFont = m_emptyFont;
    splitPtr->m_emptyColor = m_emptyColor;
  }
  else if (charIndex == 0) {
    m_emptyFont = m_charList[0].m_font;
    m_emptyColor = m_charList[0].m_color;
    splitPtr->m_charList = m_charList;
    m_charList.clear();
  }
  else if (charIndex == m_charList.size()) {
    splitPtr->m_emptyFont = m_charList[m_charList.size() - 1].m_font;
    splitPtr->m_emptyColor = m_charList[m_charList.size() - 1].m_color;
  }
  else {
    splitPtr->m_charList.insert(charIndex, splitPtr->m_charList);
    m_charList.erase(charIndex, m_charList.size() - 1);
  }

  return splitPtr;
}

bool ParagraphX::GetWord(int charIndex, int& firstIndex, int &lastIndex) {
  for (int index = charIndex;; --index) {
    if ((index == 0) || (!isalnum(m_charList[index].m_tChar))) {
      firstIndex = index;
      break;
    }
  }

  for (int index = charIndex;; ++index) {
    if ((index == m_charList.size()) || !isalnum(m_charList[index].m_tChar)){
      lastIndex = index - 1;
      break;
    }
  }

  auto x = m_charList.begin(), y = m_charList.end();

  bool b = (x < y);

  return (firstIndex <= lastIndex);
}

int ParagraphX::HomeChar(int charIndex) {
  int size = m_lineList.size();

  for (int charIndex = 0; charIndex < size; ++charIndex) {
    LineInfo* lineInfoPtr = m_lineList[charIndex];
    if (charIndex <= (lineInfoPtr->m_last + 1)) {
      return lineInfoPtr->m_first;
    }
  }

  return 0;
}

int ParagraphX::EndChar(int charIndex) {
  int size = m_lineList.size();

  for (int charIndex = 0; charIndex < size; ++charIndex) {
    LineInfo* lineInfoPtr = m_lineList[charIndex];
    if (charIndex <= (lineInfoPtr->m_last + 1)) {
      return (lineInfoPtr->m_last + 1);
    }
  }

  return 0;
}

void ParagraphX::SwitchPageBreak() {
  m_pageBreak = !m_pageBreak;
  m_yStartPos = -1;
}
 
int ParagraphX::MouseDown(Point mousePoint) {
  if (m_charList.empty()) {
    return 0;
  }
  else {
    int lineHeight = 0;
    mousePoint -= Size(0, m_yStartPos);
    mousePoint.Y = min(m_height - 1, mousePoint.Y);

    for each (LineInfo* lineInfoPtr in m_lineList) {
      lineHeight += lineInfoPtr->m_height;

      if (mousePoint.Y < lineHeight) {
        CharInfo firstChar = m_charList[lineInfoPtr->m_first],
                 lastChar = m_charList[lineInfoPtr->m_last];

        if (mousePoint.X <= firstChar.m_charRect.Left) {
          return lineInfoPtr->m_first;
        }
        else if (mousePoint.X >= lastChar.m_charRect.Right) {
          return lineInfoPtr->m_last + 1;
        }
        else {
          for (int charIndex = lineInfoPtr->m_first; charIndex <= lineInfoPtr->m_last;
               ++charIndex) {
            CharInfo charInfo = m_charList[charIndex];

            if (mousePoint.X < charInfo.m_charRect.Right) {
//              return charIndex;

              int leftSize = mousePoint.X - charInfo.m_charRect.Left,
                  rightSide = charInfo.m_charRect.Right - mousePoint.X;
              return (leftSize < rightSide) ? charIndex : (charIndex + 1);
            }
          }
        }
      }
    }

    assert(FALSE);
    return 0;
  }
}

Rect ParagraphX::CaretArea(int charIndex) {
  Rect caretRect;
  Point topLeft(0, m_yStartPos);

  if (m_charList.empty()) {
    caretRect = m_emptyRect + topLeft;
  }
  else if (charIndex == m_charList.size()) {
    Rect lastRect = m_charList[m_charList.size() - 1].m_charRect;
    caretRect = Rect(lastRect.Right, lastRect.Top, lastRect.Right +
                     lastRect.Width(), lastRect.Bottom) + topLeft;
  }
  else {
    caretRect = m_charList[charIndex].m_charRect + topLeft;
  }

  if (m_wordDocumentPtr->CheckKeyboardState() == InsertKeyboard) {
    caretRect.Right = caretRect.Left + 1;
  }

  if (caretRect.Right > PageWidth()) {
    int difference = caretRect.Right - PageWidth();
    caretRect.Left -= difference;
    caretRect.Right -= difference;
  }

  return caretRect;
}

pair<int,int> ParagraphX::LineArea(int charIndex) {
  if (m_charList.empty()) {
    return pair<int,int>(m_yStartPos, m_yStartPos + m_height);
  }
  else if (charIndex == m_charList.size()) {
    LineInfo lastLine = m_lineList[m_lineList.size() - 1];
    return pair<int,int>(m_yStartPos + m_height - lastLine.m_height,
                         m_yStartPos + m_height);
  }
  else {
    int currHeight = 0;

    for each (LineInfo* lineInfoPtr in m_lineList) {
      if ((charIndex <= lineInfoPtr->m_last) ||
          (charIndex == (lineInfoPtr->m_last + 1) &&
          (m_charList[lineInfoPtr->m_last + 1].m_tChar == TEXT(' ')))) {
        return pair<int,int>(m_yStartPos + currHeight,
                             m_yStartPos + currHeight + lineInfoPtr->m_height);
      }

      currHeight += lineInfoPtr->m_height;
    }
  }

  return pair<int,int>(0, 0);
}
#endif