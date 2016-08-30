#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

void WordDocument::OnChar(TCHAR tChar) {
  if (isprint(tChar) || (tChar == NewLine)) {
    if (wordMode == WordMark) {
      OnDelete();
    }

    Paragraph* paragraphPtr = charList[editIndex].ParagraphPtr();

    switch (GetKeyboardMode()) {
      case InsertKeyboard:
        OnInsertChar(tChar, paragraphPtr);
        break;

      case OverwriteKeyboard:
        OnOverwriteChar(tChar, paragraphPtr);
        break;
    }

    SetDirty(true);
    GenerateParagraph(paragraphPtr);
    CalculateDocument();
    
    if (MakeVisible()) {
      Invalidate();
      UpdateWindow();
    }

    UpdateCaret();
  }
}

void WordDocument::OnInsertChar(TCHAR tChar,
                                Paragraph* paragraphPtr) {
  if (nextFont != SystemFont) {
    charList.Insert(editIndex++,
                    CharInfo(paragraphPtr, tChar, nextFont));
    ClearNextFont();
  }
  else if (charList[editIndex].ParagraphPtr()->First() <
           editIndex) {
    Font font = charList[editIndex - 1].CharFont();
    charList.Insert(editIndex++,
                    CharInfo(paragraphPtr, tChar, font));
  }
  else {
    Font font = charList[editIndex].CharFont();
    charList.Insert(editIndex++,
                    CharInfo(paragraphPtr, tChar, font));
  }

  ++paragraphPtr->Last();

  for (int parIndex = paragraphPtr->Index() + 1;
       parIndex <= paragraphList.Size() - 1; ++parIndex) {
    ++paragraphList[parIndex]->First();
    ++paragraphList[parIndex]->Last();
  }
}

void WordDocument::OnOverwriteChar(TCHAR tChar,
                                   Paragraph* paragraphPtr) {
  if (editIndex == (charList.Size() - 1)) {
    if (nextFont != SystemFont) {
      charList.Insert(editIndex++,
        CharInfo(paragraphPtr, tChar, nextFont));
      charList[editIndex] =
        CharInfo(paragraphPtr, NewLine, nextFont);
      ClearNextFont();
    }
    else {
      Font font = charList[editIndex].CharFont();
      charList.Insert(editIndex++,
                      CharInfo(paragraphPtr, tChar, font));
    }

    ++paragraphPtr->Last();
  }
  else {
    if (nextFont != SystemFont) {
      charList[editIndex++] =
        CharInfo(paragraphPtr, tChar, nextFont);
      ClearNextFont();
    }
    else {
      Font font = charList[editIndex].CharFont();
      charList[editIndex++] = CharInfo(paragraphPtr, tChar, font);
    }
  }
}

void WordDocument::ClearNextFont() {
  if (nextFont != SystemFont) {
    nextFont = SystemFont;
    Paragraph* paragraphPtr = charList[editIndex].ParagraphPtr();
    GenerateParagraph(paragraphPtr);
    CalculateDocument();
    UpdateWindow();
  }
}

bool WordDocument::OnKeyDown(WORD key, bool shiftPressed,
                             bool /* controlPressed */) {
  switch (key) {
    case KeyLeft:
    case KeyRight:
    case KeyUp:
    case KeyDown:
    case KeyHome:
    case KeyEnd: {
        if (shiftPressed) {
          OnShiftKey(key);
        }
        else {
          OnRegularKey(key);
        }
      }
      return true;

    case KeyBackspace:
    case KeyReturn:
      OnNeutralKey(key);
      return true;
  }

  return false;
}

void WordDocument::OnRegularKey(WORD key) {
  EnsureEditStatus();

  switch (key) {
    case KeyLeft:
      OnLeftArrowKey();
      break;

    case KeyRight:
      OnRightArrowKey();
      break;

    case KeyUp:
      OnUpArrowKey();
      break;

    case KeyDown:
      OnDownArrowKey();
      break;

    case KeyHome:
      OnHomeKey();
      break;

    case KeyEnd:
      OnEndKey();
      break;
  }

  if (MakeVisible()) {
    Invalidate();
    UpdateWindow();
  }

  UpdateCaret();
}

void WordDocument::EnsureEditStatus() {
  if (wordMode == WordMark) {
    wordMode = WordEdit;
    editIndex = min(lastMarkIndex, charList.Size() - 1); /// XXX
    InvalidateBlock(firstMarkIndex, lastMarkIndex);
    UpdateCaret();
    UpdateWindow();
  }
}

void WordDocument::OnLeftArrowKey() {
  if (editIndex > 0) {
    ClearNextFont();
    --editIndex;
  }
}

void WordDocument::OnRightArrowKey() {
  if (editIndex < (charList.Size() - 1)) {
    ClearNextFont();
    ++editIndex;
  }
}

void WordDocument::OnUpArrowKey() {
  CharInfo charInfo = charList[editIndex];

  Paragraph* paragraphPtr = charInfo.ParagraphPtr();
  Point topLeft(0, paragraphPtr->Top());

  LineInfo* lineInfoPtr = charInfo.LineInfoPtr();
  Rect lineRect =
    topLeft + Rect(0, lineInfoPtr->Top(), PageInnerWidth(),
                      lineInfoPtr->Top() + lineInfoPtr->Height());

  if (lineRect.Top() > 0) {
    ClearNextFont();
    Rect charRect = topLeft + charInfo.CharRect();
    editIndex =
      MousePointToIndex(Point(charRect.Left(), lineRect.Top() - 1));
  }
}

void WordDocument::OnDownArrowKey() {
  CharInfo charInfo = charList[editIndex];

  Paragraph* paragraphPtr = charInfo.ParagraphPtr();
  Point topLeft(0, paragraphPtr->Top());

  LineInfo* lineInfoPtr = charInfo.LineInfoPtr();
  Rect lineRect =
    topLeft + Rect(0, lineInfoPtr->Top(), PageInnerWidth(),
                      lineInfoPtr->Top() + lineInfoPtr->Height());

  Paragraph* lastParagraphPtr = paragraphList.Back();
  int bottom = lastParagraphPtr->Top() +
               lastParagraphPtr->Height();

  if (lineRect.Bottom() < bottom) {
    ClearNextFont();
    Rect charRect = topLeft + charInfo.CharRect();
    editIndex =
      MousePointToIndexDown(Point(charRect.Left(),
                                  lineRect.Bottom() + 1));
  }
}

int WordDocument::MousePointToIndexDown(Point mousePoint) const {
  for (int parIndex = 0; parIndex < paragraphList.Size();
       ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];

    if (mousePoint.Y() <=
        (paragraphPtr->Top() + paragraphPtr->Height())) {
      return MousePointToParagraphIndex
             (paragraphList[parIndex], mousePoint);
    }
  }

  assert(false);
  return 0;
}

void WordDocument::OnPageUpKey() {
  CharInfo charInfo = charList[editIndex];
  Rect editRect = charInfo.CharRect();

  Paragraph* paragraphPtr = charInfo.ParagraphPtr();
  Point topLeft(0, paragraphPtr->Top());

  int scrollPage = GetVerticalScrollPageHeight();
  Point editPoint((editRect.Left() + editRect.Right()) / 2,
        ((editRect.Top() + editRect.Bottom()) / 2) - scrollPage);

  editIndex = MousePointToIndex(topLeft + editPoint);
}

void WordDocument::OnPageDownKey() {
  CharInfo charInfo = charList[editIndex];
  Rect editRect = charInfo.CharRect();

  Paragraph* paragraphPtr = charInfo.ParagraphPtr();
  Point topLeft(0, paragraphPtr->Top());

  int scrollPage = GetVerticalScrollPageHeight();
  Point editPoint((editRect.Left() + editRect.Right()) / 2,
        ((editRect.Top() + editRect.Bottom()) / 2) + scrollPage);

  editIndex = MousePointToIndex(topLeft + editPoint);
}

void WordDocument::OnHomeKey() {
  CharInfo charInfo = charList[editIndex];
  int homeCharIndex = charInfo.ParagraphPtr()->First() +
                      charInfo.LineInfoPtr()->First();

  if (homeCharIndex < editIndex) {
    ClearNextFont();
    editIndex = homeCharIndex;
    UpdateCaret();
  }
}

void WordDocument::OnEndKey() {
  CharInfo charInfo = charList[editIndex];
  int endCharIndex = charInfo.ParagraphPtr()->First() +
                     charInfo.LineInfoPtr()->Last();

  if (editIndex < endCharIndex) {
    ClearNextFont();
    editIndex = endCharIndex;
    UpdateCaret();
  }
}

void WordDocument::OnShiftKey(WORD key) {
  EnsureMarkStatus();

  switch (key) {
    case KeyLeft:
      OnShiftLeftArrowKey();
      break;

    case KeyRight:
      OnShiftRightArrowKey();
      break;

    case KeyUp:
      OnShiftUpArrowKey();
      break;

    case KeyDown:
      OnShiftDownArrowKey();
      break;

    case KeyPageUp:
      OnShiftPageUpKey();
      break;

    case KeyPageDown:
      OnShiftPageDownKey();
      break;

    case KeyHome:
      OnShiftHomeKey();
      break;

    case KeyEnd:
      OnShiftEndKey();
      break;
  }

  if (MakeVisible()) {
    Invalidate();
    UpdateWindow();
  }

  UpdateCaret();
}

void WordDocument::EnsureMarkStatus() {
  if (wordMode == WordEdit) {
    ClearNextFont();
    wordMode = WordMark;
    firstMarkIndex = editIndex;
    lastMarkIndex = editIndex;
    UpdateCaret();
  }
}

void WordDocument::OnShiftLeftArrowKey() {
  if (lastMarkIndex > 0) {
    InvalidateBlock(lastMarkIndex, --lastMarkIndex);
  }
}

void WordDocument::OnShiftRightArrowKey() {
  if (lastMarkIndex < charList.Size()) {
    InvalidateBlock(lastMarkIndex, lastMarkIndex++);
  }
}

void WordDocument::OnShiftUpArrowKey() {
  CharInfo charInfo = charList[lastMarkIndex];

  Paragraph* paragraphPtr = charInfo.ParagraphPtr();
  Point topLeft(0, paragraphPtr->Top());

  LineInfo* lineInfoPtr = charInfo.LineInfoPtr();
  Rect lineRect =
    topLeft + Rect(0, lineInfoPtr->Top(), PageInnerWidth(),
                      lineInfoPtr->Top() + lineInfoPtr->Height());

  if ((paragraphPtr->Top() + lineRect.Top()) > 0) {
    Rect charRect = topLeft + charInfo.CharRect();
    int newLastMarkIndex =
      MousePointToIndex(Point(charRect.Left(), lineRect.Top() - 1));
    InvalidateBlock(lastMarkIndex, newLastMarkIndex);
    lastMarkIndex = newLastMarkIndex;
  }
}

void WordDocument::OnShiftDownArrowKey() {
  CharInfo charInfo = charList[lastMarkIndex];

  Paragraph* paragraphPtr = charInfo.ParagraphPtr();
  Point topLeft(0, paragraphPtr->Top());

  LineInfo* lineInfoPtr = charInfo.LineInfoPtr();
  Rect lineRect = topLeft + Rect(0, lineInfoPtr->Top(), PageInnerWidth(),
                                 lineInfoPtr->Top() + lineInfoPtr->Height());

  Paragraph* lastParagraphPtr = paragraphList.Back();
  int bottom = lastParagraphPtr->Top() + lastParagraphPtr->Height();

  if (lineRect.Bottom() < bottom) {
    Rect charRect = topLeft + charInfo.CharRect();
    int newLastMarkIndex =
      MousePointToIndexDown(Point(charRect.Left(),
                                  lineRect.Bottom() + 1));
    InvalidateBlock(lastMarkIndex, newLastMarkIndex);
    lastMarkIndex = newLastMarkIndex;
  }
}

void WordDocument::OnShiftPageUpKey() {
  Rect lastRectMark = charList[lastMarkIndex].CharRect();
  int scrollPage = GetVerticalScrollPageHeight();
  Point lastPointMark
    ((lastRectMark.Left() + lastRectMark.Right()) / 2,
     (lastRectMark.Top()+lastRectMark.Bottom()) / 2 - scrollPage);

  int newLastMarkIndex = MousePointToIndex(lastPointMark);
  InvalidateBlock(lastMarkIndex, newLastMarkIndex);
  lastMarkIndex = newLastMarkIndex;
}

void WordDocument::OnShiftPageDownKey() {
  Rect lastRectMark = charList[lastMarkIndex].CharRect();

  int scrollPage = GetVerticalScrollPageHeight();
  Point lastPointMark
    ((lastRectMark.Left() + lastRectMark.Right()) / 2,
     (lastRectMark.Top()+lastRectMark.Bottom())/2 + scrollPage);

  int newLastMarkIndex = MousePointToIndexDown(lastPointMark);
  InvalidateBlock(lastMarkIndex, newLastMarkIndex);
  lastMarkIndex = newLastMarkIndex;
}

void WordDocument::OnShiftHomeKey() {
  CharInfo charInfo = charList[editIndex];
  int homeCharIndex = charInfo.ParagraphPtr()->First() +
                      charInfo.LineInfoPtr()->First();

  if (homeCharIndex < lastMarkIndex) {
    InvalidateBlock(lastMarkIndex, homeCharIndex);
    lastMarkIndex = homeCharIndex;
  }
}

void WordDocument::OnShiftEndKey() {
  CharInfo charInfo = charList[editIndex];
  int endCharIndex = charInfo.ParagraphPtr()->First() +
                     charInfo.LineInfoPtr()->Last();

  if (lastMarkIndex < endCharIndex) {
    InvalidateBlock(lastMarkIndex, endCharIndex);
    lastMarkIndex = endCharIndex;
  }
}

void WordDocument::OnControlHomeKey() {
  EnsureEditStatus();

  if (editIndex > 0) {
    editIndex = 0;

    if (MakeVisible()) {
      Invalidate();
      UpdateWindow();
    }

    UpdateCaret();
  }
}

void WordDocument::OnControlEndKey() {
  EnsureEditStatus();

  if (editIndex < (charList.Size() - 1)) {
    editIndex = charList.Size() - 1;

    if (MakeVisible()) {
      Invalidate();
      UpdateWindow();
    }

    UpdateCaret();
  }
}

void WordDocument::OnShiftControlHomeKey() {
  EnsureMarkStatus();
  ClearNextFont();

  if (lastMarkIndex > 0) {
    InvalidateBlock(0, lastMarkIndex);
    lastMarkIndex = 0;

    if (MakeVisible()) {
      Invalidate();
      UpdateWindow();
    }

    UpdateCaret();
  }
}

void WordDocument::OnShiftControlEndKey() {
  EnsureMarkStatus();

  if (lastMarkIndex < (charList.Size() - 1)) {
    int lastIndex = charList.Size() - 1;
    InvalidateBlock(lastMarkIndex, lastIndex);
    lastMarkIndex = lastIndex;

    if (MakeVisible()) {
      Invalidate();
      UpdateWindow();
    }

    UpdateCaret();
  }
}

void WordDocument::OnNeutralKey(WORD key) {
  switch (key) {
    case KeyBackspace:
      OnBackspaceKey();
      break;

    case KeyReturn:
      OnReturnKey();
      break;
  }

  if (MakeVisible()) {
    Invalidate();
    UpdateWindow();
  }

  UpdateCaret();
}

void WordDocument::OnReturnKey() {
  OnChar(NewLine);

  Paragraph* firstParagraphPtr =
    charList[editIndex].ParagraphPtr();
  Paragraph* secondParagraphPtr =
    new Paragraph(editIndex, firstParagraphPtr->Last(),
                  firstParagraphPtr->AlignmentField(),
                  firstParagraphPtr->Index() + 1);
  assert(firstParagraphPtr != nullptr);
  firstParagraphPtr->Last() = editIndex - 1;

  for (int charIndex = secondParagraphPtr->First();
       charIndex <= secondParagraphPtr->Last(); ++charIndex) {
    charList[charIndex].ParagraphPtr() = secondParagraphPtr;
  }

  GenerateParagraph(firstParagraphPtr);
  GenerateParagraph(secondParagraphPtr);
  paragraphList.Insert(firstParagraphPtr->Index() + 1,
                       secondParagraphPtr);

  for (int parIndex = secondParagraphPtr->Index() + 1;
       parIndex < paragraphList.Size(); ++parIndex) {
    ++paragraphList[parIndex]->Index();
  }

  SetDirty(true);
  CalculateDocument();
  UpdateCaret();
  UpdateWindow();
}

void WordDocument::OnBackspaceKey() {
  switch (wordMode) {
    case WordEdit:
      if (editIndex > 0) {
        OnLeftArrowKey();
        OnDelete();
      }
      break;

    case WordMark:
      OnDelete();
      break;
  }
}

bool WordDocument::MakeVisible() {
  Rect visibleArea;

  switch (wordMode) {
    case WordEdit: {
        Paragraph* editParagraphPtr =
          charList[editIndex].ParagraphPtr();
        Point topLeft(0, editParagraphPtr->Top());
        visibleArea = topLeft + charList[editIndex].CharRect();
      }
      break;

    case WordMark: {
        Paragraph* lastParagraphPtr =
          charList[max(0, lastMarkIndex - 1)].ParagraphPtr();
        Point topLeft(0, lastParagraphPtr->Top());
        visibleArea =
          topLeft + charList[max(0, lastMarkIndex - 1)].CharRect();
      }
      break;
  }

  int horiScrollLeft = GetHorizontalScrollPosition(),
      vertScrollTop = GetVerticalScrollPosition();
  Size clientSize = GetClientSize();
  int horiScrollRight = horiScrollLeft + clientSize.Width(),
      vertScrollBottom = vertScrollTop + clientSize.Height();

  if (visibleArea.Left() < horiScrollLeft) {
    SetHorizontalScrollPosition(visibleArea.Left());
    return true;
  }

  if (visibleArea.Top() < vertScrollTop) {
    SetVerticalScrollPosition(visibleArea.Top());
    return true;
  }

  if (visibleArea.Right() > horiScrollRight) {
    int horiDifference = visibleArea.Right() - horiScrollRight;
    SetHorizontalScrollPosition(horiScrollLeft + horiDifference);
    return true;
  }

  if (visibleArea.Bottom() > vertScrollBottom) {
    int vertDifference = visibleArea.Bottom() - vertScrollBottom;
    SetVerticalScrollPosition(vertScrollTop + vertDifference);
    return true;
  }

  return false;
}