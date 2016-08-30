#include "SmallWindows.h"

namespace SmallWindows {
  const Size USLetterPortrait(21590, 10000), LineSize(500, 500);
//  const Size USLetterPortrait(21590, 27940), LineSize(500, 500);

  Document::Document(CoordinateSystem system, Size pageSize,
                     Window* parentPtr /* = nullptr */,
                     WindowStyle style /* = OverlappedWindow */,
                     WindowShow windowShow /* = Normal */,
                     bool acceptDropFiles /* = true */,
                     Size lineSize /* = LineSize */)
   :Document::Document(TEXT("document"), system, pageSize,
                       parentPtr, style, windowShow,
                       acceptDropFiles, lineSize) {
    // Empty.
  }

  Document::Document(String className, CoordinateSystem system,
                     Size pageSize, Window* parentPtr/*=nullptr*/,
                     WindowStyle style /* = OverlappedWindow */,
                     WindowShow windowShow /* = Normal */,
                     bool acceptDropFiles /* = true */,
                     Size lineSize /* = LineSize */)
   :Window(className, system, pageSize, parentPtr,
           style, NoStyle, windowShow),
    lineSize(lineSize) {
    GenerateHeader();

    SetHorizontalScrollPosition(0);
    SetVerticalScrollPosition(0);

    SetHorizontalScrollTotalWidth(pageSize.Width());
    SetVerticalScrollTotalHeight(pageSize.Height());

    Size clientSize = GetClientSize();
    SetHorizontalScrollPageWidth(clientSize.Width());
    SetVerticalScrollPageHeight(clientSize.Height());

    ::DragAcceptFiles(windowHandle,
                      acceptDropFiles ? TRUE : FALSE);
  }

  Document::~Document() {
    if (caretPresent) {
      ::DestroyCaret();
    }
  }

  String Document::GetName() const {
    return name;
  }

  void Document::SetName(String name) {
    this->name = name;
    GenerateHeader();
  }

  void Document::SetZoom(double zoom) {
    Window::SetZoom(zoom);
    GenerateHeader();
  }

  bool Document::IsDirty() const {
    return dirtyFlag;
  }

  void Document::SetDirty(bool dirty) {
    dirtyFlag = dirty;
    GenerateHeader();
  }

  void Document::GenerateHeader() {
    String headerName = name.empty() ? TEXT("[No Name]") : name,
           dirtyText = dirtyFlag ? TEXT("*") : TEXT("");
    int zoomPerCent = (int) (100 * GetZoom());

    if (zoomPerCent!= 100) {
      String zoomText =
        TEXT(" ") + to_String(zoomPerCent) + TEXT("%");
      SetHeader(headerName + dirtyText + zoomText);
    }
    else {
      SetHeader(headerName + dirtyText);
    }
  }

  void Document::OnSize(Size clientSize) {
    SetHorizontalScrollPageWidth(clientSize.Width());
    SetVerticalScrollPageHeight(clientSize.Height());
  }

  void Document::OnGainFocus() {
    if (caretPresent) {
      ::ShowCaret(windowHandle);
    }
  }

  void Document::OnLoseFocus() {
    if (caretPresent) {
      ::HideCaret(windowHandle);
    }
  }

  void Document::SetCaret(Rect caretLogicalRect) {
    if (caretPresent) {
      ::DestroyCaret();
    }

    Rect deviceCaretRect = LogicalToDevice(caretLogicalRect);
    if (deviceCaretRect.Width() == 0) {      
      deviceCaretRect.Right() = deviceCaretRect.Left() + 1;
    }

    ::CreateCaret(windowHandle, nullptr, deviceCaretRect.Width(),
                  deviceCaretRect.Height());
    ::SetCaretPos(deviceCaretRect.Left(), deviceCaretRect.Top());
    ::ShowCaret(windowHandle);

    caretPresent = true;
  }

  void Document::ClearCaret() {
    if (caretPresent) {
      ::DestroyCaret();
    }

    caretPresent = false;
  }

  void Document::OnMouseWheel(WheelDirection wheelDirection,
                         bool shiftPressed, bool controlPressed) {
    if (controlPressed) {
      switch (wheelDirection) {
        case WheelUp:
          SetZoom(min(10.0, 1.11 * GetZoom()));
          break;

        case WheelDown:
          SetZoom(max(0.1, 0.9 * GetZoom()));
          break;
      }
    }
    else {
      switch (wheelDirection) {
        case WheelUp:
          OnVerticalScroll(SB_LINEUP);
          break;

        case WheelDown:
          OnVerticalScroll(SB_LINEDOWN);
          break;
      }
    }

    Invalidate();
    UpdateWindow();
    UpdateCaret();
  }

  void Document::SetMenuBar(Menu& menuBar) {
    menuBarHandle = menuBar.menuHandle;
    ::SetMenu(windowHandle, menuBarHandle);
  }

  void Document::OnCommand(WORD commandId) {
    Command command = commandMap[commandId];
    command.Selection()(this);
  }

  void Document::OnCommandInit() {
    for (pair<WORD,Command> pair : commandMap) {
      WORD commandId = pair.first;
      Command command = pair.second;

      if (command.Enable() != nullptr) {
        UINT enableFlag = command.Enable()(this) ?
                          MF_ENABLED : MF_GRAYED;
        ::EnableMenuItem(menuBarHandle, commandId,
                         MF_BYCOMMAND | enableFlag);
      }

      { bool checkFlag = false;
        if (command.Check() != nullptr) {
          BoolListener checkListener = command.Check();
          checkFlag = checkListener(this);
        }
        
        bool radioFlag = false;
        if (command.Radio() != nullptr) {
          BoolListener radioListener = command.Radio();
          radioFlag = radioListener(this);
        }
        
        UINT checkFlags = (checkFlag | radioFlag) ?
                          MF_CHECKED : MF_UNCHECKED;
        ::CheckMenuItem(menuBarHandle, commandId,
                        MF_BYCOMMAND | checkFlags);
      }
    }
  }

  void Document::OnHorizontalScroll(WORD flags,
                                    WORD thumbPos /*= 0 */) {
    int scrollPos = GetHorizontalScrollPosition(),
        scrollLine = GetHorizontalScrollLineWidth(),
        scrollPage = GetHorizontalScrollPageWidth(),
        scrollSize = GetHorizontalScrollTotalWidth();

    switch (flags) {
      case SB_LEFT:
        SetHorizontalScrollPosition(0);
        break;

      case SB_LINELEFT:
        SetHorizontalScrollPosition(max(0, scrollPos -
                                           scrollLine));
        break;

      case SB_PAGELEFT:
        SetHorizontalScrollPosition(max(0, scrollPos -
                                           scrollPage));
        break;

      case SB_LINERIGHT:
        SetHorizontalScrollPosition(min(scrollPos + scrollLine,
                                        scrollSize - scrollLine));
        break;

      case SB_PAGERIGHT:
        SetHorizontalScrollPosition(min(scrollPos + scrollLine,
                                        scrollSize - scrollPage));
        break;

      case SB_RIGHT:
        SetHorizontalScrollPosition(scrollSize - scrollPage);
        break;

      case SB_THUMBTRACK:
      case SB_THUMBPOSITION:
        SetHorizontalScrollPosition(thumbPos);
        break;
    }
  }

  void Document::OnVerticalScroll(WORD flags,
                                  WORD thumbPos /* = 0 */) {
    int scrollPos = GetVerticalScrollPosition(),
        scrollLine = GetVerticalScrollLineHeight(),
        scrollPage = GetVerticalScrollPageHeight(),
        scrollSize = GetVerticalScrollTotalHeight();

    switch (flags) {
      case SB_TOP:
        SetVerticalScrollPosition(0);
        break;

      case SB_LINEUP:
        SetVerticalScrollPosition(max(0, scrollPos - scrollLine));
        break;

      case SB_PAGEUP:
        SetVerticalScrollPosition(max(0, scrollPos - scrollPage));
        break;

      case SB_LINEDOWN:
        SetVerticalScrollPosition(min(scrollPos + scrollLine,
                                      scrollSize - scrollLine));
        break;

      case SB_PAGEDOWN:
        SetVerticalScrollPosition(min(scrollPos + scrollLine,
                                      scrollSize - scrollPage));
        break;

      case SB_BOTTOM:
        SetVerticalScrollPosition(scrollSize - scrollPage);
        break;

      case SB_THUMBTRACK:
      case SB_THUMBPOSITION:
        SetVerticalScrollPosition(thumbPos);
        break;
    }
  }

  bool Document::KeyToScroll(WORD key, bool shiftPressed,
                             bool controlPressed) {
    switch (key) {
      case KeyUp:
        OnVerticalScroll(SB_LINEUP);
        return true;

      case KeyDown:
        OnVerticalScroll(SB_LINEDOWN);
        return true;

      case KeyPageUp:
        OnVerticalScroll(SB_PAGEUP);
        return true;

      case KeyPageDown:
        OnVerticalScroll(SB_PAGEDOWN);
        return true;

      case KeyLeft:
        OnHorizontalScroll(SB_LINELEFT);
        return true;

      case KeyRight:
        OnHorizontalScroll(SB_LINERIGHT);
        return true;

      case KeyHome:
        OnHorizontalScroll(SB_LEFT);
        if (controlPressed) {
          OnVerticalScroll(SB_TOP);
        }
        return true;

      case KeyEnd:
        OnHorizontalScroll(SB_RIGHT);
        if (controlPressed) {
          OnVerticalScroll(SB_BOTTOM);
        }
        return true;
    }

    return false;
  }

  void Document::SetHorizontalScrollPosition(int scrollPos) {
    if (scrollPos != GetHorizontalScrollPosition()) {
      ::SetScrollPos(windowHandle, SB_HORZ, scrollPos, TRUE);
      Invalidate();
      UpdateWindow();
      UpdateCaret();
    }
  }

  int Document::GetHorizontalScrollPosition() const {
    return ::GetScrollPos(windowHandle, SB_HORZ);
  }

  void Document::SetVerticalScrollPosition(int scrollPos) {
    if (scrollPos != GetVerticalScrollPosition()) {
      ::SetScrollPos(windowHandle, SB_VERT, scrollPos, TRUE);
      Invalidate();
      UpdateWindow();
      UpdateCaret();
    }
  }

  int Document::GetVerticalScrollPosition() const {
    return ::GetScrollPos(windowHandle, SB_VERT);
  }

  void Document::SetHorizontalScrollLineWidth(int lineWidth) {
    lineSize.Width() = lineWidth;
  }

  int Document::GetHorizontalScrollLineWidth() const {
    return lineSize.Width();
  }

  void Document::SetVerticalScrollLineHeight(int lineHeight) {
    lineSize.Height() = lineHeight;
  }

  int Document::GetVerticalScrollLineHeight() const {
    return lineSize.Height();
  }

  void Document::SetHorizontalScrollPageWidth(int pageWidth) {
    SCROLLINFO scrollInfo = {sizeof(SCROLLINFO), SIF_PAGE};
    scrollInfo.nPage = pageWidth;
    ::SetScrollInfo(windowHandle, SB_HORZ, &scrollInfo, TRUE);
  }

  int Document::GetHorizontalScrollPageWidth() const {
    SCROLLINFO scrollInfo = {sizeof(SCROLLINFO), SIF_PAGE};
    ::GetScrollInfo(windowHandle, SB_HORZ, &scrollInfo);
    return scrollInfo.nPage;
  }

  void Document::SetVerticalScrollPageHeight(int pageHeight) {
    SCROLLINFO scrollInfo = {sizeof(SCROLLINFO), SIF_PAGE};
    scrollInfo.nPage = pageHeight;
    ::SetScrollInfo(windowHandle, SB_VERT, &scrollInfo, TRUE);
  }

  int Document::GetVerticalScrollPageHeight() const {
    SCROLLINFO scrollInfo = {sizeof(SCROLLINFO), SIF_PAGE};
    ::GetScrollInfo(windowHandle, SB_VERT, &scrollInfo);
    return scrollInfo.nPage;
  }

  void Document::SetHorizontalScrollTotalWidth(int scrollWidth) {
    ::SetScrollRange(windowHandle, SB_HORZ, 0, scrollWidth, TRUE);
  }

  int Document::GetHorizontalScrollTotalWidth() const {
    int minRange, maxRange;
    ::GetScrollRange(windowHandle, SB_HORZ, &minRange, &maxRange);
    return maxRange;
  }

  void Document::SetVerticalScrollTotalHeight(int scrollHeight) {
    ::SetScrollRange(windowHandle, SB_VERT, 0, scrollHeight,TRUE);
  }

  int Document::GetVerticalScrollTotalHeight() const {
    int minRange, maxRange;
    ::GetScrollRange(windowHandle, SB_VERT, &minRange, &maxRange);
    return maxRange;
  }

  LRESULT CALLBACK DocumentProc(HWND windowHandle, UINT message,
                                WPARAM wordParam, LPARAM longParam) {
    if ((windowHandle != nullptr) && (WindowMap.count(windowHandle) == 1)) {
      Document* documentPtr =
        dynamic_cast<Document*>(WindowMap[windowHandle]);

      if (documentPtr != nullptr) {
        switch (message) {
          case WM_MOUSEWHEEL: {
              bool down = (HIWORD(wordParam) & 0x0100) != 0;
              WheelDirection wheelDirection =
                down ? WheelDown : WheelUp;
              bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
              bool controlPressed = (::GetKeyState(VK_CONTROL)<0);
              documentPtr->OnMouseWheel(wheelDirection,
                             shiftPressed, controlPressed);
            }
            return 0;

          case WM_KEYDOWN: {
            WORD key = wordParam;

              if (key == KeyInsert) {
                switch (documentPtr->GetKeyboardMode()) {
                  case InsertKeyboard:
                    documentPtr->
                      SetKeyboardMode(OverwriteKeyboard);
                    documentPtr->
                      OnKeyboardMode(OverwriteKeyboard);
                    break;

                  case OverwriteKeyboard:
                    documentPtr->SetKeyboardMode(InsertKeyboard);
                    documentPtr->OnKeyboardMode(InsertKeyboard);
                    break;
                }

                return 0;
              }
              else {
                bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
                bool controlPressed=(::GetKeyState(VK_CONTROL)<0);

                if (documentPtr->OnKeyDown(wordParam,shiftPressed,
                                           controlPressed) ||
                    documentPtr->KeyToScroll(key, shiftPressed,
                                             controlPressed)) {
                  return 0;
                }
              }
            }
            break;

          case WM_COMMAND:
            documentPtr->OnCommand(LOWORD(wordParam));
            return 0;

          case WM_INITMENUPOPUP:
            documentPtr->OnCommandInit();
            return 0;

          case WM_DROPFILES: {
              vector<String> pathList =
                ExtractPathList(wordParam);
              documentPtr->OnDropFile(pathList);
            }
            return 0;

          case WM_HSCROLL: {
              WORD flags = LOWORD(wordParam),
                   thumbPos = HIWORD(wordParam);
              documentPtr->OnHorizontalScroll(flags, thumbPos);
            }
            return 0;

          case WM_VSCROLL: {
              WORD flags = LOWORD(wordParam),
                   thumbPos = HIWORD(wordParam);
              documentPtr->OnVerticalScroll(flags, thumbPos);
            }
            return 0;
        }
      }
    }

    return WindowProc(windowHandle, message,
                      wordParam, longParam);
  }

  vector<String> ExtractPathList(WORD wordParam) {
    vector<String> pathList;
    HDROP dropHandle = (HDROP)wordParam;
    int size =
      ::DragQueryFile(dropHandle, 0xFFFFFFFF, nullptr, 0);

    for (int index = 0; index < size; ++index) {
      int bufferSize = 
        ::DragQueryFile(dropHandle, index, nullptr, 0) + 1;
      TCHAR* path = new TCHAR[bufferSize];
      assert(path!= nullptr);

      assert(::DragQueryFile(dropHandle, index,
                              path, bufferSize) != 0);
      pathList.push_back(String(path));
      delete [] path;
    }

    return pathList;
  }
};