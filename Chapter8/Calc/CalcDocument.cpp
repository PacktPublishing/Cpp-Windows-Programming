#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"
#include "Cell.h"
#include "CalcDocument.h"

CalcDocument::CalcDocument(WindowShow windowShow)
 :StandardDocument(LogicalWithoutScroll, USLetterPortrait,
                   TEXT("Calc Files, clc; Text Files, txt"),
                   nullptr, OverlappedWindow, windowShow,
                   {CalcFormat, UnicodeFormat, AsciiFormat},
                   {CalcFormat, UnicodeFormat, AsciiFormat}) {
  Menu menuBar(this);
  menuBar.AddMenu(StandardFileMenu(false));
  menuBar.AddMenu(StandardEditMenu());

  Menu formatMenu(this, TEXT("F&ormat"));
  formatMenu.AddItem(TEXT("&Font ...\tCtrl+F"), OnFont);
  formatMenu.AddItem(TEXT("&Background Color ...\tCtrl+B"),
                     OnBackgroundColor);

  Menu horizontalMenu(this, TEXT("&Horizontal Alignment"));
  horizontalMenu.AddItem(TEXT("&Left"), OnHorizontalLeft,
                         nullptr, nullptr, HorizontalLeftRadio);
  horizontalMenu.AddItem(TEXT("&Center"), OnHorizontalCenter,
                         nullptr, nullptr, HorizontalCenterRadio);
  horizontalMenu.AddItem(TEXT("&Right"), OnHorizontalRight,
                         nullptr, nullptr, HorizontalRightRadio);
  horizontalMenu.AddItem(TEXT("&Justified"),OnHorizontalJustified,                            
                    nullptr, nullptr, HorizontalJustifiedRadio);

  Menu verticalMenu(this, TEXT("&Vertical Alignment"));
  verticalMenu.AddItem(TEXT("&Top"), OnVerticalTop,
                       nullptr, nullptr, VerticalTopRadio);
  verticalMenu.AddItem(TEXT("&Center"), OnVerticalCenter,
                       nullptr, nullptr, VerticalCenterRadio);
  verticalMenu.AddItem(TEXT("&Bottom"), OnVerticalBottom,
                       nullptr, nullptr, VerticalBottomRadio);

  formatMenu.AddMenu(horizontalMenu);
  formatMenu.AddMenu(verticalMenu);
  menuBar.AddMenu(formatMenu);

  menuBar.AddMenu(StandardHelpMenu());
  SetMenuBar(menuBar);

/*Menu formatMenu(this);
  formatMenu.AddItem(TEXT("&Font ...\tCtrl+F"), OnFont);
  formatMenu.AddItem(TEXT("&Background Color ...\tCtrl+B"),
                     OnBackgroundColor);

  Menu horizontalMenu(this);
  horizontalMenu.AddItem(TEXT("&Left"), OnHorizontalLeft,
                         nullptr, nullptr, HorizontalLeftRadio);
  horizontalMenu.AddItem(TEXT("&Center"), OnHorizontalCenter,
                         nullptr, nullptr, HorizontalCenterRadio);
  horizontalMenu.AddItem(TEXT("&Right"), OnHorizontalRight,
                         nullptr, nullptr, HorizontalRightRadio);
  horizontalMenu.AddItem(TEXT("&Justified"), OnHorizontalJustified,
                         nullptr, nullptr, HorizontalJustifiedRadio);

  Menu verticalMenu(this);
  verticalMenu.AddItem(TEXT("&Top"), OnVerticalTop,
                       nullptr, nullptr, VerticalTopRadio);
  verticalMenu.AddItem(TEXT("&Center"), OnVerticalCenter,
                       nullptr, nullptr, VerticalCenterRadio);
  verticalMenu.AddItem(TEXT("&Bottom"), OnVerticalBottom,
                       nullptr, nullptr, VerticalBottomRadio);

  formatMenu.AddMenu(horizontalMenu, TEXT("&Horizontal Alignment"));
  formatMenu.AddMenu(verticalMenu, TEXT("&Vertical Alignment "));
  AddToStandardMenuBar(formatMenu));*/

  for (int row = 0; row < Rows; ++row) {
    for (int col = 0; col < Cols; ++col) {
      auto x = &cellMatrix[row][col];
      cellMatrix[Reference(row, col)].GenerateCaretList(this);
    }
  }
}

void CalcDocument::OnMouseDown(MouseButton mouseButtons,
                   Point mousePoint, bool shiftPressed /*=false*/,
                   bool controlPressed /* = false */) {
  if ((calcMode == Mark) || ToMarkMode()) {
    markOk = true;
    Reference newFirstMarkRef;
    ClickArea clickArea = GetMouseLocation(mousePoint,
                                           newFirstMarkRef);
    MarkBlock(clickArea, newFirstMarkRef, newFirstMarkRef);
    UpdateCaret();
  }
  else {
    markOk = false;
  }
}

void CalcDocument::OnMouseMove(MouseButton mouseButtons,
                   Point mousePoint, bool shiftPressed /*=false*/,
                   bool controlPressed /* = false */) {
  if ((mouseButtons == LeftButton) && markOk) {
    Reference newLastMarkRef;
    ClickArea clickArea =
      GetMouseLocation(mousePoint, newLastMarkRef);
    MarkBlock(clickArea, firstMarkRef, newLastMarkRef);
  }
}

void CalcDocument::OnDoubleClick(MouseButton mouseButtons,
                   Point mousePoint, bool shiftPressed /*=false*/,
                   bool controlPressed /* = false */) {
  if ((mouseButtons == LeftButton) && markOk) {
    ClickArea clickArea = GetMouseLocation(mousePoint, editRef);

    if (clickArea == ClickCell) {
      calcMode = Edit;
      Cell& editCell = cellMatrix[editRef];
      prevCell = editCell;
      editCell.DisplayFormula();
      editCell.GenerateCaretList(this);
      editIndex =
        editCell.MouseToIndex(mousePoint.X() % ColWidth);
      InvalidateCell(editRef);
      UpdateWindow();
      UpdateCaret();
    }
  }
}

void CalcDocument::OnVerticalScroll(WORD flags, WORD y) {
  int row = y / RowHeight;
  SetVerticalScrollPosition(row * RowHeight);
}

void CalcDocument::OnHorizontalScroll(WORD flags, WORD x) {
  int col = x / ColWidth;
  SetHorizontalScrollPosition(col * ColWidth);
}

CalcDocument::ClickArea CalcDocument::GetMouseLocation
                   (Point mousePoint, Reference& cellRef) const {
  if ((mousePoint.X() <= HeaderWidth) &&
      (mousePoint.Y() <= HeaderHeight)) {
    return ClickAll;
  }
  else if (mousePoint.X() <= HeaderWidth) {
    mousePoint.Y() += GetVerticalScrollPosition() - HeaderHeight;
    cellRef = Reference(min(Rows-1, mousePoint.Y()/RowHeight), 0);
    return ClickRow;
  }
  else if (mousePoint.Y() <= HeaderHeight) {
    mousePoint.X() += GetHorizontalScrollPosition() - HeaderWidth;
    cellRef = Reference(0, min(Cols - 1,
                               mousePoint.X() / ColWidth));
    return ClickColumn;
  }
  else {
    mousePoint.X() += GetHorizontalScrollPosition() - HeaderWidth;
    mousePoint.Y() += GetVerticalScrollPosition() - HeaderHeight;
    cellRef = Reference(min(Rows - 1, mousePoint.Y() / RowHeight),
                        min(Cols - 1, mousePoint.X() / ColWidth));
    return ClickCell;
  }
}

void CalcDocument::MarkBlock(ClickArea clickArea,
         Reference newFirstMarkRef, Reference newLastMarkRef) {
  switch (clickArea) {
    case ClickAll:
      Remark(ZeroReference, Reference(Rows - 1, Cols - 1));
      break;

    case ClickRow:
      Remark(Reference(newFirstMarkRef.Row(), 0),
             Reference(newLastMarkRef.Row(), Cols - 1));
      break;

    case ClickColumn:
      Remark(Reference(0, newFirstMarkRef.Col()),
             Reference(Rows - 1, newLastMarkRef.Col()));
      break;

    case ClickCell:
      Remark(newFirstMarkRef, newLastMarkRef);
      break;
  }
}

void CalcDocument::OnDraw(Graphics& graphics,
                          DrawMode /* drawMode */) const {
  int horizontalScroll = GetHorizontalScrollPosition(),
      verticalScroll = GetVerticalScrollPosition();

  int startRow = horizontalScroll / RowHeight,
      startCol = verticalScroll / ColWidth;

  graphics.DrawRectangle(Rect(0, 0, HeaderWidth, HeaderHeight), 
                         Black);

  for (int col = startCol; col < Cols; ++col) {
    int x = (col * ColWidth) - horizontalScroll + HeaderWidth;
    Rect headerRect(x, 0, x + ColWidth, HeaderHeight);
    graphics.DrawRectangle(Rect(x, 0, x + ColWidth, HeaderHeight),
                           Black);
    TCHAR buffer[] = {(TCHAR) (TEXT('A') + col), TEXT('\0')};
    graphics.DrawText(headerRect, buffer,
                      SystemFont, Black, White);
  }

  for (int row = startRow; row < Rows; ++row) {
    int y = (row * RowHeight) - verticalScroll + HeaderHeight;
    Rect headerRect(0, y, HeaderWidth, y + RowHeight);
    graphics.DrawRectangle(Rect(0, y, HeaderWidth, y + RowHeight),
                           Black);
    String buffer = to_String(row + 1);
    graphics.DrawText(headerRect, buffer,
                      SystemFont, Black, White);
  }

  int minMarkRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
      minMarkCol = min(firstMarkRef.Col(), lastMarkRef.Col()),
      maxMarkRow = max(firstMarkRef.Row(), lastMarkRef.Row()),
      maxMarkCol = max(firstMarkRef.Col(), lastMarkRef.Col());

  for (int row = startRow; row < Rows; ++row) {
    for (int col = startCol; col < Cols; ++col) {
      bool edit = (calcMode == Edit) &&
                  (row == editRef.Row())&&(col == editRef.Col());

      bool mark = (calcMode == Mark) &&
                  (row >= minMarkRow) && (row <= maxMarkRow) &&
                  (col >= minMarkCol) && (col <= maxMarkCol);

      Reference cellRef(row, col);
      Cell cell = cellMatrix[cellRef];
      cell.DrawCell(graphics, cellRef, edit || mark);
    }
  }
}

bool CalcDocument::IsCellVisible(Reference cellRef) const {
  int horizontalScrollPos = GetHorizontalScrollPosition(),
      verticalScrollPos = GetVerticalScrollPosition();
  int firstVisibleCol = verticalScrollPos / ColWidth,
      firstVisibleRow = horizontalScrollPos / RowHeight;

  Size clientSize = GetClientSize();
  int lastVisibleCol =
        (verticalScrollPos + clientSize.Width()) / ColWidth,
      lastVisibleRow =
        (horizontalScrollPos + clientSize.Height()) / RowHeight;

  int row = cellRef.Row(), col = cellRef.Col();
  return (row >= firstVisibleRow) && (row <= lastVisibleRow) &&
         (col >= firstVisibleCol) && (col <= lastVisibleCol);
}

void CalcDocument::MakeCellVisible() {
  switch (calcMode) {
    case Edit:
      MakeCellVisible(editRef);
      break;

    case Mark:
      MakeCellVisible(lastMarkRef);
      break;
  }
}

void CalcDocument::MakeCellVisible(Reference cellRef) {
  Point topLeft(cellRef.Col() * ColWidth,
                cellRef.Row() * RowHeight);
  Rect cellRect(topLeft, Size(ColWidth, RowHeight));
  Size clientSize = GetClientSize();

  if (clientSize.Width() < (HeaderWidth + Cols * ColWidth)) {
    int left = GetHorizontalScrollPosition(),
        xPage = GetHorizontalScrollPageWidth();
    int right = left + xPage - 1;

    if (cellRect.Left() < left) {
      SetHorizontalScrollPosition(cellRect.Left());
      Invalidate();
      UpdateWindow();
    }

    if (cellRect.Right() > right) {
      int distance = cellRect.Right() - right;
      distance += ColWidth - distance % ColWidth;
      SetHorizontalScrollPosition(left + distance);
      Invalidate();
      UpdateWindow();
    }
  }

  if (clientSize.Height() < (HeaderHeight + Rows * RowHeight)) {
    int top = GetHorizontalScrollPosition(),
        yPage = GetHorizontalScrollPageWidth();
    int bottom = top + yPage - 1;

    if (cellRect.Top() < top) {
      SetVerticalScrollPosition(cellRect.Top());
      Invalidate();
      UpdateWindow();
    }

    if (cellRect.Bottom() > bottom) {
      int distance = cellRect.Bottom() - bottom;
      distance += RowHeight - distance % RowHeight;
      SetVerticalScrollPosition(top + distance);
      Invalidate();
      UpdateWindow();
    }
  }
}

void CalcDocument::UpdateCaret() {
  if ((calcMode == Edit) && IsCellVisible(editRef)) {
    Point topLeft(HeaderWidth + (editRef.Col() * ColWidth) +
                  CellMargin, HeaderHeight + (editRef.Row() *
                  RowHeight) + CellMargin);
    Cell& editCell = cellMatrix[editRef];
    Rect caretRect = editCell.CaretList()[editIndex];

    if (GetKeyboardMode() == InsertKeyboard) {
      caretRect.Right() = caretRect.Left() + 1;
    }

    SetCaret(topLeft + caretRect);
  }
  else {
    ClearCaret();
  }
}

bool CalcDocument::ToMarkMode() {
  if (calcMode == Edit) {
    if (InterpretEditCell()) {
      calcMode = Mark;
      firstMarkRef = editRef;
      lastMarkRef = editRef;
      return true;
    }

    return false;
  }

  return true;
}

/*void CalcDocument::ClearMarked(Reference newFirstRef, Reference newLastRef) {
  Reference minMarked(min(firstMarkRef.Row(), lastMarkRef.Row()),
                      min(firstMarkRef.Col(), lastMarkRef.Col())),
            maxMarked(max(firstMarkRef.Row(), lastMarkRef.Row()),
                      max(firstMarkRef.Col(), lastMarkRef.Col()));

  for (int row = minMarked.Row(); row <= maxMarked.Row(); ++row) {
    for (int col = minMarked.Col(); col <= maxMarked.Col(); ++col) {
      Reference cellRef(row, col);
      InvalidateCell(cellRef);
    }
  }
}*/

void CalcDocument::Remark(Reference newFirstRef,
                          Reference newLastRef) {
  Reference
    minOldMarked(min(firstMarkRef.Row(), lastMarkRef.Row()),
                 min(firstMarkRef.Col(), lastMarkRef.Col())),
    maxOldMarked(max(firstMarkRef.Row(), lastMarkRef.Row()),
                 max(firstMarkRef.Col(), lastMarkRef.Col())),
    minNewMarked(min(newFirstRef.Row(), newLastRef.Row()),
                 min(newFirstRef.Col(), newLastRef.Col())),
    maxNewMarked(max(newFirstRef.Row(), newLastRef.Row()),
                 max(newFirstRef.Col(), newLastRef.Col()));

  for (int row = minOldMarked.Row();
       row <= maxOldMarked.Row(); ++row) {
    for (int col = minOldMarked.Col();
         col <= maxOldMarked.Col(); ++col) {
      Reference cellRef(row, col);
      if (!cellRef.Inside(minNewMarked, maxNewMarked)) {
        InvalidateCell(cellRef);
      }
    }
  }

  for (int row = minNewMarked.Row();
       row <= maxNewMarked.Row(); ++row) {
    for (int col = minNewMarked.Col();
         col <= maxNewMarked.Col(); ++col) {
      Reference cellRef(row, col);
      if (!cellRef.Inside(minOldMarked, maxOldMarked)) {
        InvalidateCell(Reference(row, col));
      }
    }
  }

  firstMarkRef = newFirstRef;
  lastMarkRef = newLastRef;
  UpdateWindow();
}