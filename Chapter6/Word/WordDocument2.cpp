//       1         2         3         4         5         6         7
//345678901234567890123456789012345678901234567890123456789012345678901234567

#include "..\\SmallWindows\\SmallWindows.h"
#include "Position.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

Window* MainWindow(vector<String> /* argumentList */,
                   WindowShow windowShow) {
  Application::Name = TEXT("Word");
  return (new WordDocument(windowShow));
}

const Font WordDocument::BreakFont(TEXT("Times New Roman"), 12, false, true);
const unsigned int WordDocument::WordFormat = 1001;

WordDocument::WordDocument(WindowShow windowShow)
 :StandardDocument(LogicalWithoutScroll, Size(21600, 27900),TEXT("wrd"),NULL,
                   OverlappedWindow, windowShow,
                   {WordFormat, UnicodeFormat, AsciiFormat},
                   {WordFormat, UnicodeFormat, AsciiFormat}),
  m_wordStatus(WordEdit),
  m_nextFont(TEXT("Times New Roman"), 32),
  m_currColor(SystemColor) {

  Menu menuBar(this);
  SetMenuBar(menuBar);

  menuBar.AddMenu(StandardFileMenu(true));

  Menu editMenu = StandardEditMenu();
  editMenu.AddSeparator();
  editMenu.AddItem(TEXT("&Page Break\tCtrl+B"), OnPageBreak);
  menuBar.AddMenu(editMenu);

  Menu formatMenu(this, TEXT("F&ormat"));
  formatMenu.AddItem(TEXT("&Font\tCtrl+F"), OnFont);
  Menu zoomMenu(this, TEXT("&Zoom"));
  zoomMenu.AddItem(TEXT("&Increase\tCtrl+I"), OnIncrease);
  zoomMenu.AddItem(TEXT("&Decrease\tCtrl+D"), OnDecrease);
  formatMenu.AddMenu(zoomMenu);
  menuBar.AddMenu(formatMenu);

  Menu alignmentMenu(this, TEXT("&Alignment"));
  alignmentMenu.AddItem(TEXT("&Left\tCtrl+L"), OnLeft,
                        NULL, NULL, LeftRadio);
  alignmentMenu.AddItem(TEXT("&Center\tCtrl+E"), OnCenter,
                        NULL, NULL, CenterRadio);
  alignmentMenu.AddItem(TEXT("&Right\tCtrl+R"), OnRight,
                        NULL, NULL, RightRadio);
  alignmentMenu.AddItem(TEXT("&Justified\tCtrl+J"), OnJustified,
                        NULL, NULL, JustifiedRadio);
  menuBar.AddMenu(alignmentMenu);

  menuBar.AddMenu(StandardHelpMenu());

//  m_nextFont.ReadFromRegistry(TEXT("m_nextFont"));
  Create();
  UpdateCaret();
}

WordDocument::~WordDocument() {
  m_nextFont.WriteToRegistry(TEXT("m_nextFont"));
}

void WordDocument::OnPageOrientation() {
  for each (Paragraph* paragraphPtr in m_paraList) {
    paragraphPtr->Calculate();
  }

  CheckDocument();
  Invalidate();
  UpdateWindow();
}

vector<Paragraph*>::iterator WordDocument::IndexToPosition(int index) {
  vector<Paragraph*>::iterator iter = m_paraList.begin();

  while (((index--) > 0) && (iter != m_paraList.end())) {
    ++iter;
  }

  return iter;
}

void WordDocument::UpdateCaret() {
  switch (m_wordStatus) {
    case WordEdit: {
        Rect caretRect = m_paraList[m_psEdit.m_paragraph]->
                         CaretArea(m_psEdit.m_char);
        SetCaret(caretRect);
      }
      break;

    case WordMark:
      SetCaret(ZeroRect);
      break;
  }
}

void WordDocument::InvalidateRectSet(set<Rect>& rectSet) {
  if (!rectSet.empty()) {
    for each (Rect charRect in rectSet) {
      Invalidate(charRect);
    }

    UpdateWindow();
  }
}

void WordDocument::OnMouseDown(MouseButton mouseButtons, Point mousePoint,
                               bool shiftPressed, bool controlPressed) {
  if ((mouseButtons & LeftButton) != 0) {
    EnsureEditStatus();
    m_wordStatus = WordMark;
    m_psFirstMark = m_psOldLastMark=m_psLastMark=MouseToPosition(mousePoint);
    m_nextFont = SystemFont;  
    m_currColor = SystemColor;
    UpdateCaret();
  }
}

void WordDocument::OnMouseMove(MouseButton mouseButtons, Point mousePoint,
                               bool shiftPressed, bool controlPressed) {
  if ((mouseButtons & LeftButton) != 0) {
    m_psLastMark = MouseToPosition(mousePoint);

    if (m_psOldLastMark != m_psLastMark) {
      UpdateMarkedArea(m_psFirstMark, m_psOldLastMark, m_psLastMark);
      UpdateCaret();
      m_psOldLastMark = m_psLastMark;
    }
  }
}

void WordDocument::OnMouseUp(MouseButton mouseButtons, Point mousePoint,
                             bool shiftPressed, bool controlPressed) {
  if ((mouseButtons & LeftButton) != 0) {
    if (m_psFirstMark == m_psLastMark) {
      m_wordStatus = WordEdit;
      m_psEdit = m_psFirstMark;
      UpdateCaret();
    }
  }
}

void WordDocument::OnDoubleClick(MouseButton mouseButtons, Point mousePoint,
                                 bool shiftPressed, bool controlPressed) {
  int firstIndex, lastIndex;
  Paragraph* paragraphPtr = m_paraList[m_psEdit.m_paragraph];

  if (paragraphPtr->GetWord(m_psEdit.m_char, firstIndex, lastIndex)) {
    set<Rect> rectSet;

    m_wordStatus = WordMark;
    m_psFirstMark.m_paragraph = m_psEdit.m_paragraph;
    m_psLastMark.m_paragraph = m_psEdit.m_paragraph;
    m_psFirstMark.m_char = firstIndex;
    m_psLastMark.m_char = lastIndex;

    UpdateCaret();
    GetRepaintSet(rectSet, m_psFirstMark, m_psLastMark);
    InvalidateRectSet(rectSet);
  }
}

void WordDocument::OnTouchDown(vector<Point> pointList) {
  if (pointList.size() == 2) {
    m_initZoom = GetZoom();
    Point firstInitPoint = pointList[0], secondInitPoint = pointList[1];
    double xInitDistance = firstInitPoint.X - secondInitPoint.X,
           yInitDistance = firstInitPoint.Y - secondInitPoint.Y,
    m_initDistance = sqrt((xInitDistance * xInitDistance) +
                          (yInitDistance * yInitDistance));
  }
  else {
    Window::OnTouchDown(pointList);
  }
}

void WordDocument::OnTouchMove(vector<Point> pointList) {
  if (pointList.size() == 2) {
    Point firstPoint = pointList[0], secondPoint = pointList[1];

    int xDistance = firstPoint.X - secondPoint.X,
        yDistance = firstPoint.Y - secondPoint.Y;
    double distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));

    double factor = distance / m_initDistance;
    SetZoom(min(factor * m_initZoom, MAX_ZOOM));

    UpdateCaret();
    Invalidate();
    UpdateWindow();
  }
  else {
    Window::OnTouchMove(pointList);
  }
}

void WordDocument::OnPaint(Graphics& graphics) {
  { String breakText = TEXT("-- Page Break --");
    Size breakSize(min(PageWidth(), GetHorizontalScrollPage()),
                       GetCharacterHeight(BreakFont));

    for (int page = 1; page < ((int) m_pageList.size()); ++page) {
      Point topLeft(0, page * PageHeight());
      Rect breakRect(topLeft, breakSize);
      graphics.DrawText(breakRect, breakText, BreakFont, Black, White);
    }
  }

  OnDraw(graphics);
}

void WordDocument::OnDraw(Graphics& graphics) {
  Position psMinMark = Min(m_psFirstMark, m_psLastMark),
           psMaxMark = Max(m_psFirstMark, m_psLastMark);

  int size = m_paraList.size();
  for (int paragraph = 0; paragraph < size; ++paragraph) {
    Paragraph* paragraphPtr = m_paraList[paragraph];

    switch (m_wordStatus) {
      case WordEdit:
        paragraphPtr->Draw(graphics, 0, -1);
        break;

      case WordMark: {
          if ((paragraph == psMinMark.m_paragraph) &&
              (paragraph == psMaxMark.m_paragraph)) {
            paragraphPtr->Draw(graphics, psMinMark.m_char, psMaxMark.m_char);
          }
          else if (paragraph == psMinMark.m_paragraph) {
            paragraphPtr->Draw(graphics, psMinMark.m_char,
                               paragraphPtr->Length() - 1);
          }
          else if ((paragraph > psMinMark.m_paragraph) &&
                   (paragraph < psMaxMark.m_paragraph)) {
            paragraphPtr->Draw(graphics, 0, paragraphPtr->Length() - 1);
          }
          else if (paragraph == psMaxMark.m_paragraph) {
            paragraphPtr->Draw(graphics, 0, psMaxMark.m_char);
          }
          else {
            paragraphPtr->Draw(graphics, 0, -1);
          }
        }
        break;
    }
  }
}

void WordDocument::OnPageSetup(PrintSetupInfo pageSetupInfo) {
  for each (Paragraph* paragraphPtr in m_paraList) {
    paragraphPtr->Calculate();
  }

  CheckDocument();
  UpdateCaret();
  Invalidate();
  UpdateWindow();
}

void WordDocument::CheckDocument() {
  set<Rect> rectSet;
  int oldPageListSize = m_pageList.size();
  m_pageList.clear();
  int startPara = 0, pageHeight = 0;

  int para, size = m_paraList.size();
  for (para = 0; para < size; ++para) {
    Paragraph* paragraphPtr = m_paraList[para];

    if ((!paragraphPtr->PageBreak()) &&
        (pageHeight + paragraphPtr->Height()) <= PageHeight()) {
      pageHeight += paragraphPtr->Height();
    }
    else {
      PageInfo pageInfo(startPara, para - 1, pageHeight);
      m_pageList.push_back(pageInfo);
      startPara = para;
      pageHeight = paragraphPtr->Height();
    }
  }

  PageInfo pageInfo(startPara, para - 1, pageHeight);
  m_pageList.push_back(pageInfo);

  int size2 = m_pageList.size();
  for (int page = 0; page < size2; ++page) {
    int yScrollPage = page * PageHeight();
    PageInfo pageInfo = m_pageList[page];

    for (int para = pageInfo.m_firstParagraph;
         para <= pageInfo.m_lastParagraph; ++para) {
      Paragraph* paragraphPtr = m_paraList[para];

      if (yScrollPage != paragraphPtr->StartPos()) {
        Rect newParaRect(0, yScrollPage, PageWidth(),
                         yScrollPage + paragraphPtr->Height());
        rectSet.insert(newParaRect);

        if (paragraphPtr->StartPos() != -1) {
          Rect oldParagraphRect(0, paragraphPtr->StartPos(), PageWidth(),
                           paragraphPtr->StartPos()+paragraphPtr->Height());
          rectSet.insert(oldParagraphRect);
        }

        paragraphPtr->SetStartPos(yScrollPage);
      }

      yScrollPage += paragraphPtr->Height();
    }

    if (page < oldPageListSize)) {
      PageInfo oldPageInfo = oldPageList[page];

      if (oldPageInfo.m_height > pageInfo.m_height) {
        Rect pageDiffRect(0, pageInfo.m_height,
                          PageWidth(), oldPageInfo.m_height);
        rectSet.insert(pageDiffRect);
      }
    }
  }

  if (m_pageList.size() != oldPageListSize) {
    int minListSize = min(m_pageList.size(), oldPageList.size()),
        maxListSize = max(m_pageList.size(), oldPageList.size());

    Size pageBreakSize = GetPageBreakSize();
    Rect restDocumentRect(0, minListSize * PageHeight() - pageBreakSize.CY,
                        PageWidth(), maxListSize * PageHeight());
    rectSet.insert(restDocumentRect);
    int a = GetVerticalScrollPosition();

    SetVerticalScrollSize(m_pageList.size() * PageHeight());
    Invalidate();
  }
  else {
    InvalidateRectSet(rectSet);
  }

  UpdateCaret();
  UpdateWindow();
}

Size WordDocument::GetPageBreakSize() {  
  int width = GetTextWidth(BreakFont, TEXT("-- Page Break --")),
      height = GetCharacterHeight(BreakFont);
  return Size(width, height);
}

void WordDocument::Delete(set<Rect>& repaintSet) {
  Position psMin = Min(m_psFirstMark, m_psLastMark),
           psMax = Max(m_psFirstMark, m_psLastMark);

  if (psMin.m_paragraph == psMax.m_paragraph) {
    Paragraph* minParagraphPtr = m_paraList[psMin.m_paragraph];
    minParagraphPtr->Delete(psMin.m_char, psMax.m_char - 1);
    minParagraphPtr->Calculate(&repaintSet);
  }
  else {
    Paragraph *minParagraphPtr = m_paraList[psMin.m_paragraph],
              *maxParagraphPtr = m_paraList[psMax.m_paragraph];
    minParagraphPtr->Delete(psMin.m_char);
    maxParagraphPtr->Delete(0, psMax.m_char);
    minParagraphPtr->Append(maxParagraphPtr);

    auto paraPos = IndexToPosition(psMax.m_paragraph);
    for (int para = psMax.m_paragraph; para > psMin.m_paragraph; --para) {
      delete m_paraList[psMax.m_paragraph];
      m_paraList.erase(paraPos--);
    }
  }
}
