#include "SmallWindows.h"

namespace SmallWindows {
  PrintPreviewDocument::PrintPreviewDocument
    (StandardDocument* parentDocument, int totalPages /* = 1 */,
     Size pageSize/* = USLetterPortrait */)
   :Document(PreviewCoordinate, pageSize, parentDocument),
    page(1),
    totalPages(totalPages) {

    SetHorizontalScrollPosition(0);
    SetHorizontalScrollPageWidth(pageSize.Width());
    SetHorizontalScrollTotalWidth(pageSize.Width());
  
    SetVerticalScrollPosition(0);
    SetVerticalScrollPageHeight(pageSize.Height());
    SetVerticalScrollTotalHeight(totalPages * pageSize.Height());

    SetHeader();
    SetWindowPosition(Point(0, 0));
    ShowWindow(true);
  }

  void PrintPreviewDocument::SetHeader() {
    SetName(TEXT("Print Preview: Page ") + to_String(page) +
            TEXT(" out of ") + to_String(totalPages));
  }

  bool PrintPreviewDocument::OnKeyDown
    (WORD key, bool shiftPressed, bool controlPressed) {
    switch (key) {
      case KeyEscape: {
          Window* parentWindow = ParentWindowPtr();
          ::CloseWindow(WindowHandle());
          parentWindow->SetFocus();
        }
        break;

      case KeyHome:
        OnVerticalScroll(SB_TOP);
        break;

      case KeyEnd:
        OnVerticalScroll(SB_BOTTOM);
        break;

      case KeyUp:
      case KeyPageUp:
        OnVerticalScroll(SB_LINEUP);
        break;

      case KeyDown:
      case KeyPageDown:
        OnVerticalScroll(SB_LINEDOWN);
        break;
    }

    return true;
  }

  void PrintPreviewDocument::OnVerticalScroll(WORD flags,
                                       WORD thumbPos /* = 0 */) {
    int oldPage = page;

    switch (flags) {
      case SB_LINEUP:
      case SB_PAGEUP:
        page = max(1, page - 1);
        break;

      case SB_LINEDOWN:
      case SB_PAGEDOWN:
        page = min(page + 1, totalPages);
        break;

      case SB_THUMBTRACK:
      case SB_THUMBPOSITION:
        page = (thumbPos / pageSize.Height()) + 1;
        break;

      case SB_TOP:
        page = 1;
        break;

      case SB_BOTTOM:
        page = totalPages;
        break;
    }

    if (oldPage != page) {
      SetHeader();
      SetVerticalScrollPosition((page - 1) * pageSize.Height());
      Invalidate();
      UpdateWindow();
    }
  }

  void PrintPreviewDocument::OnPaint(Graphics& graphics) const {
    StandardDocument* parentDocument =
      (StandardDocument*) ParentWindowPtr();
    parentDocument->OnPrint(graphics, page, 1, totalPages);
  }
};