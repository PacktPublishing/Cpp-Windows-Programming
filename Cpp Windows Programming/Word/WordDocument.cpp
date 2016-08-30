#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

WordDocument::WordDocument(WindowShow windowShow)
 :StandardDocument(LogicalWithScroll, USLetterPortrait,
                   TEXT("Word Files, wrd; Text Files, txt"),
                   nullptr, OverlappedWindow, windowShow,
                   {WordFormat, UnicodeFormat, AsciiFormat},
                   {WordFormat, UnicodeFormat, AsciiFormat}) {
  Menu menuBar(this);
  menuBar.AddMenu(StandardFileMenu(true));
  menuBar.AddMenu(StandardEditMenu());

  Menu formatMenu(this, TEXT("F&ormat"));
  formatMenu.AddItem(TEXT("&Font\tCtrl+F"), OnFont);
  formatMenu.AddItem(TEXT("&Page Break\tCtrl+B"),
                     OnPageBreak, PageBreakEnable);
  menuBar.AddMenu(formatMenu);

  Menu alignmentMenu(this, TEXT("&Alignment"));
  alignmentMenu.AddItem(TEXT("&Left\tCtrl+L"), OnLeft,
                        nullptr, nullptr, LeftRadio);
  alignmentMenu.AddItem(TEXT("&Center\tCtrl+E"), OnCenter,
                        nullptr, nullptr, CenterRadio);
  alignmentMenu.AddItem(TEXT("&Right\tCtrl+R"), OnRight,
                        nullptr, nullptr, RightRadio);
  alignmentMenu.AddItem(TEXT("&Justified\tCtrl+J"), OnJustified,
                        nullptr, nullptr, JustifiedRadio);
  menuBar.AddMenu(alignmentMenu);

  menuBar.AddMenu(StandardHelpMenu());
  SetMenuBar(menuBar);

  Menu extraMenu(this);
  extraMenu.AddItem(TEXT("&A\tCtrl+Home"), OnControlHomeKey);
  extraMenu.AddItem(TEXT("&B\tCtrl+End"), OnControlEndKey);
  extraMenu.AddItem(TEXT("&C\tShift+Ctrl+Home"),
                    OnShiftControlHomeKey);
  extraMenu.AddItem(TEXT("&D\tShift+Ctrl+End"),
                    OnShiftControlEndKey);
  InitDocument();
}

void WordDocument::InitDocument() {
  wordMode = WordEdit;
  editIndex = 0;
  Paragraph* firstParagraphPtr = new Paragraph(0, 0, Left, 0);
  assert(firstParagraphPtr != nullptr);
  Font font(TEXT("Times New Roman"), 36, false, true);
  charList.PushBack(CharInfo(firstParagraphPtr, NewLine, font));
  GenerateParagraph(firstParagraphPtr);
  paragraphList.PushBack(firstParagraphPtr);
  CalculateDocument();
  UpdateCaret();
}

WordDocument::~WordDocument() {
  // Empty.
}

void WordDocument::OnKeyboardMode(KeyboardMode/*=KeyboardMode*/) {
  UpdateCaret();
}

void WordDocument::UpdateCaret() {
  switch (wordMode) {
    case WordEdit: {
        CharInfo charInfo = charList[editIndex];
        Rect caretRect = charList[editIndex].CharRect();

        if (nextFont != SystemFont) {
          int width = GetCharacterAverageWidth(nextFont),
              height = GetCharacterHeight(nextFont);
          caretRect.Right() = caretRect.Left() + width;
          caretRect.Top() = caretRect.Bottom() - height;
        }
        else if ((GetKeyboardMode() == InsertKeyboard) &&
                 (charInfo.ParagraphPtr()->First() < editIndex)) {
          Rect prevCharRect = charList[editIndex - 1].CharRect();
          caretRect.Top() = caretRect.Bottom() -
                            prevCharRect.Height();
        }

        if (GetKeyboardMode() == InsertKeyboard) {
          caretRect.Right() = caretRect.Left() + 1;
        }

        if (caretRect.Right() >= PageInnerWidth()) {
          caretRect.Right() = PageInnerWidth() - 1;
        }

        Paragraph* paragraphPtr =
          charList[editIndex].ParagraphPtr();
        Point topLeft = Point(0, paragraphPtr->Top());
        SetCaret(topLeft + caretRect);
      }
      break;

    case WordMark:
      ClearCaret();
      break;
  }
}

void WordDocument::OnMouseDown(MouseButton mouseButtons,
                          Point mousePoint, bool shiftPressed,
                          bool controlPressed) {
  if (mouseButtons == LeftButton) {
    EnsureEditStatus();
    ClearNextFont();
    wordMode = WordMark;
    firstMarkIndex = lastMarkIndex =
      MousePointToIndex(mousePoint);
    UpdateCaret();
  }
}

void WordDocument::OnMouseMove(MouseButton mouseButtons,
                          Point mousePoint, bool shiftPressed,
                          bool controlPressed) {
  if (mouseButtons == LeftButton) {
    int newLastMarkIndex = MousePointToIndex(mousePoint);

    if (lastMarkIndex != newLastMarkIndex) {
      InvalidateBlock(lastMarkIndex, newLastMarkIndex);
      lastMarkIndex = newLastMarkIndex;
    }
  }
}

void WordDocument::OnMouseUp(MouseButton mouseButtons,
                             Point mousePoint, bool shiftPressed,
                             bool controlPressed) {
  if (mouseButtons == LeftButton) {
    if (firstMarkIndex == lastMarkIndex) {
      wordMode = WordEdit;
      editIndex = min(firstMarkIndex, charList.Size() - 1);
      UpdateCaret();
    }
  }
}

int WordDocument::MousePointToIndex(Point mousePoint) const {
  for (int parIndex = 1; parIndex < paragraphList.Size();
       ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];

    if (mousePoint.Y() < paragraphPtr->Top()) {
      return MousePointToParagraphIndex
             (paragraphList[parIndex - 1], mousePoint);
    }
  }

  return MousePointToParagraphIndex
         (paragraphList[paragraphList.Size() - 1], mousePoint);
}

int WordDocument::MousePointToParagraphIndex
                (Paragraph* paragraphPtr, Point mousePoint) const{
  mousePoint.Y() -= paragraphPtr->Top();

  if (mousePoint.Y() >= paragraphPtr->Height()) {
    mousePoint.Y() = paragraphPtr->Height() - 1;
  }

  int firstChar = paragraphPtr->First();
  for (LineInfo* lineInfoPtr : paragraphPtr->LinePtrList()) {
    if (mousePoint.Y() < (lineInfoPtr->Top() +
                          lineInfoPtr->Height())) {
      Rect firstRect =
             charList[firstChar +lineInfoPtr->First()].CharRect(),
           lastRect =
             charList[firstChar + lineInfoPtr->Last()].CharRect();

      if (mousePoint.X() < firstRect.Left()) {
        return paragraphPtr->First() + lineInfoPtr->First();
      }
      else if (lastRect.Right() <= mousePoint.X()) {
        return paragraphPtr->First() + lineInfoPtr->Last();
      }
      else {
        for (int charIndex = lineInfoPtr->First();
             charIndex <= lineInfoPtr->Last(); ++charIndex) {
          Rect charRect = charList[charIndex].CharRect();

          if (mousePoint.X() < charRect.Right()) {
            int leftSize = mousePoint.X() - charRect.Left(),
                rightSide = charRect.Right() - mousePoint.X();

            return paragraphPtr->First() +
              ((leftSize < rightSide) ? charIndex
                                      : (charIndex + 1));
          }
        }
      }
    }
  }

  assert(false);
  return 0;
}

void WordDocument::InvalidateBlock(int firstIndex, int lastIndex){
  int minIndex = min(firstIndex, lastIndex),
      maxIndex = min(max(firstIndex, lastIndex),
                     charList.Size() - 1);

  for (int charIndex = minIndex; charIndex <= maxIndex;
       ++charIndex) {
    CharInfo charInfo = charList[charIndex];
    Point topLeft(0, charInfo.ParagraphPtr()->Top());
    Invalidate(topLeft + charInfo.CharRect());
  }
}

void WordDocument::OnDoubleClick(MouseButton mouseButtons,
                     Point mousePoint, bool shiftPressed,
                     bool controlPressed) {
  int firstIndex = GetFirstWordIndex(editIndex),
      lastIndex = GetLastWordIndex(editIndex);

  if (firstIndex < lastIndex) {
    wordMode = WordMark;
    firstMarkIndex = firstIndex;
    lastMarkIndex = lastIndex;

    UpdateCaret();
    InvalidateBlock(firstMarkIndex, lastMarkIndex);
    UpdateWindow();
  }
}

int WordDocument::GetFirstWordIndex(int charIndex) const {
  while ((charIndex >= 0) &&
         (isalpha(charList[charIndex].Char()))) {
    --charIndex;
  }

  return (charIndex + 1);
}

int WordDocument::GetLastWordIndex(int charIndex) const {
  while (isalpha(charList[charIndex].Char())) {
    ++charIndex;
  }

  return charIndex;
}

void WordDocument::OnTouchDown(vector<Point> pointList) {
  if (pointList.size() == 2) {
    initZoom = GetZoom();
    Point firstInitPoint = pointList[0],
          secondInitPoint = pointList[1];
    double width = firstInitPoint.X() - secondInitPoint.X(),
           height = firstInitPoint.Y() - secondInitPoint.Y(),
    initDistance = sqrt((width * width) + (height * height));
  }
  else {
    Window::OnTouchDown(pointList);
  }
}

void WordDocument::OnTouchMove(vector<Point> pointList) {
  if (pointList.size() == 2) {
    Point firstPoint = pointList[0], secondPoint = pointList[1];

    int width = firstPoint.X() - secondPoint.X(),
        height = firstPoint.Y() - secondPoint.Y();
    double distance = sqrt((width * width) + (height * height));

    double factor = distance / initDistance;
    double newZoom = factor * initZoom;
    SetZoom(min(max(newZoom, 0.1), 10.0));

    UpdateCaret();
    Invalidate();
    UpdateWindow();
  }
  else {
    Window::OnTouchMove(pointList);
  }
}

void WordDocument::OnPageSetup(PageSetupInfo pageSetupInfo) {
  ClearNextFont();

  for (Paragraph* paragraphPtr : paragraphList) {
    GenerateParagraph(paragraphPtr);
  }

  CalculateDocument();
  UpdateCaret();
  UpdateWindow();
}

void WordDocument::CalculateDocument() {
  int pageInnerWidth = PageInnerWidth(),
      pageInnerHeight = PageInnerHeight(),
      documentHeight = 0, newTotalPages = 1;

  for (int parIndex = 0; parIndex < paragraphList.Size();
       ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];

    if (paragraphPtr->Top() != documentHeight) {
      paragraphPtr->Top() = documentHeight;
      Invalidate(Rect(0, paragraphPtr->Top(), pageInnerWidth,
                 paragraphPtr->Top() + paragraphPtr->Height()));
    }

    bool pageBreak = paragraphPtr->PageBreak() &&
                     ((paragraphPtr->Top() % pageInnerHeight)!=0);

    bool notFitOnPage =
      (documentHeight > 0) &&
      ((paragraphPtr->Top() + paragraphPtr->Height()) >
       (newTotalPages * pageInnerHeight));

    if (pageBreak || notFitOnPage) {
      Rect restOfPage(0, documentHeight, pageInnerWidth,
                      newTotalPages * pageInnerHeight);
      Invalidate(restOfPage);
      paragraphPtr->Top() = (newTotalPages++) * pageInnerHeight;

      Invalidate(Rect(0, paragraphPtr->Top(), pageInnerWidth,
                 paragraphPtr->Top() + paragraphPtr->Height()));
      documentHeight = paragraphPtr->Top() +
                       paragraphPtr->Height();
    }
    else {
      documentHeight += paragraphPtr->Height();
    }
  }

  Rect restOfPage(0, documentHeight, pageInnerWidth,
                  newTotalPages * pageInnerHeight);
  Invalidate(restOfPage);

  if (totalPages != newTotalPages) {
    int minTotalPages = min(totalPages, newTotalPages),
        maxTotalPages = max(totalPages, newTotalPages);
    Invalidate(Rect(0, minTotalPages * pageInnerHeight,
               pageInnerWidth, maxTotalPages * pageInnerHeight));
    totalPages = newTotalPages;

    SetVerticalScrollPageHeight(0);
    SetVerticalScrollTotalHeight(0);
    int pageHeight = GetClientSize().Height();
    SetVerticalScrollPageHeight(pageHeight);
    int totalHeight = totalPages * pageInnerHeight;
    SetVerticalScrollTotalHeight(totalHeight);
  }
}

#if 0
void WordDocument::CalculateDocument() {
  int pageInnerWidth = PageInnerWidth(),
      pageInnerHeight = PageInnerHeight(),
      documentHeight = 0, newTotalPages = 1;

  for (int parIndex = 0; parIndex < paragraphList.Size();
       ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];

    if (paragraphPtr->Top() != documentHeight) {
      paragraphPtr->Top() = documentHeight;
      Invalidate(Rect(0, paragraphPtr->Top(), pageInnerWidth,
                 paragraphPtr->Top() + paragraphPtr->Height()));
    }

    bool pageBreak = paragraphPtr->PageBreak() &&
                     ((paragraphPtr->Top() % pageInnerHeight) != 0);

    bool notFitOnPage =
      (documentHeight > 0) &&
      ((paragraphPtr->Top() + paragraphPtr->Height()) >
       (newTotalPages * pageInnerHeight));

    if (pageBreak || notFitOnPage) {
      Rect restOfPage(0, documentHeight, pageInnerWidth,
                      newTotalPages * pageInnerHeight);
      Invalidate(restOfPage);
      paragraphPtr->Top() = (newTotalPages++) * pageInnerHeight;

      Invalidate(Rect(0, paragraphPtr->Top(), pageInnerWidth,
                 paragraphPtr->Top() + paragraphPtr->Height()));
      documentHeight = paragraphPtr->Top() +
                       paragraphPtr->Height();
    }
    else {
      documentHeight += paragraphPtr->Height();
    }
  }

  Rect restOfPage(0, documentHeight, pageInnerWidth,
                  newTotalPages * pageInnerHeight);
  Invalidate(restOfPage);

  if (totalPages != newTotalPages) {
    int minTotalPages = min(totalPages, newTotalPages),
        maxTotalPages = max(totalPages, newTotalPages);
    Invalidate(Rect(0, minTotalPages * pageInnerHeight,
                    pageInnerWidth, maxTotalPages * pageInnerHeight));
    totalPages = newTotalPages;
    SetVerticalScrollPageHeight(0);
    SetVerticalScrollTotalHeight(0);
    SetVerticalScrollPageHeight(GetClientSize().Height());
    SetVerticalScrollTotalHeight(totalPages * pageInnerHeight);
  }
}
#endif

void WordDocument::OnPaint(Graphics& graphics) const {
  int pageInnerWidth = PageInnerWidth(),
      pageInnerHeight = PageInnerHeight();
  int documentInnerHeight = totalPages * pageInnerHeight;
  Size clientSize = GetClientSize();

  if (pageInnerWidth < clientSize.Width()) {
    int maxHeight = max(documentInnerHeight, clientSize.Height());
    Rect rightRect(pageInnerWidth, 0,
                   clientSize.Width(), maxHeight);
    graphics.FillRectangle(rightRect, LightGray, LightGray);
  }

  if (documentInnerHeight < clientSize.Height()) {
    Rect bottomRect(0, documentInnerHeight,
                    pageInnerWidth, clientSize.Height());
    graphics.FillRectangle(bottomRect, LightGray, LightGray);
  }

  OnDraw(graphics, Paint);

  int breakWidth = min(pageInnerWidth, clientSize.Width()),
      breakHeight = GetCharacterHeight(SystemFont);
  Size breakSize(breakWidth, breakHeight);

  for (int pageIndex = 1; pageIndex < totalPages; ++pageIndex) {
    int line = pageIndex * pageInnerHeight;
    graphics.DrawLine(Point(0, line), Point(pageInnerWidth, line),
                      Black);

    Point topLeft(0, line - (breakHeight / 2));
    graphics.DrawText(Rect(topLeft, breakSize),
                      TEXT("Page Break"), SystemFont,Black,White);
  }
}

void WordDocument::OnDraw(Graphics& graphics,
                          DrawMode drawMode) const {
  int pageWidth = PageInnerWidth(),
      minCharIndex = min(firstMarkIndex, lastMarkIndex),
      maxCharIndex = max(firstMarkIndex, lastMarkIndex);

  for (int charIndex = 0; charIndex <= charList.Size() - 1;
       ++charIndex) {
    CharInfo charInfo = charList[charIndex];
    Point topLeft(0, charInfo.ParagraphPtr()->Top());

    Color textColor = charInfo.CharFont().FontColor();
    Color backColor = textColor.Inverse();

    if ((wordMode == WordMark) &&
        (minCharIndex <= charIndex)&&(charIndex < maxCharIndex)) {
      swap(textColor, backColor);
    }

    TCHAR tChar = (charInfo.Char() == NewLine) ? Space : charInfo.Char();
    TCHAR text[] = {tChar, TEXT('\0')};

    Rect charRect = charList[charIndex].CharRect();
    if (charRect.Right() >= pageWidth) {
      charRect.Right() = pageWidth - 1;
    }

    graphics.DrawText(topLeft + charRect, text,
                      charInfo.CharFont(), textColor, backColor);
  }

  if (drawMode == Paint) {
    for (Paragraph* paragraphPtr : paragraphList) {
      if (paragraphPtr->PageBreak()) {
        Point topLeft(0, paragraphPtr->Top());
        graphics.FillRectangle(Rect(topLeft, topLeft +
                                    Size(200, 200)), Red, Red);
      }
    }
  }
}