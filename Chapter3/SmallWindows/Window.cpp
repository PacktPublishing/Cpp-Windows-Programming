#include "SmallWindows.h"

namespace SmallWindows {
  map<HWND, Window*> WindowMap;

  Window::Window(Window* parentPtr /* = nullptr */)
   :parentPtr(parentPtr) {
    // Empty.
  }

  Window::Window(CoordinateSystem system, Size pageSize
                 /* = ZeroSize */, Window* parentPtr /*=nullptr*/,
                 WindowStyle style /* = OverlappedWindow */,
                 WindowStyle extendedStyle /* = NoStyle */,
                 WindowShow windowShow /* = Normal */,
                 Point windowTopLeft /* = ZeroPoint */,
                 Size windowSize /* = ZeroSize */)
   :Window(TEXT("window"), system, pageSize, parentPtr, style,
           extendedStyle, windowShow, windowTopLeft, windowSize) {
    // Empty.
  }

  Window::Window(String className, CoordinateSystem system,
                 Size pageSize /* = ZeroSize */,Window* parentPtr/*=nullptr*/,
                 WindowStyle style /* = OverlappedWindow */,
                 WindowStyle extendedStyle /* = NoStyle */,
                 WindowShow windowShow /* = Normal */,
                 Point windowTopLeft /* = ZeroPoint */,
                 Size windowSize /* = ZeroSize */)
   :parentPtr(parentPtr),
    system(system),
    pageSize(pageSize) {

    if (parentPtr != nullptr) {
      windowTopLeft = parentPtr->LogicalToDevice(windowTopLeft);
      windowSize = parentPtr->LogicalToDevice(windowSize);
    }

    int left, top, width, height;

    if (windowTopLeft != ZeroPoint) {
      left = windowTopLeft.X();
      top = windowTopLeft.Y();
    }
    else {
      left = CW_USEDEFAULT;
      top = CW_USEDEFAULT;
    }

    if (windowSize != ZeroSize) {
      width = windowSize.Width();
      height = windowSize.Height();
    }
    else {
      width = CW_USEDEFAULT;
      height = CW_USEDEFAULT;
    }

    HWND parentHandle = (parentPtr != nullptr) ?
                        parentPtr->windowHandle : nullptr;

    windowHandle =
      CreateWindowEx(extendedStyle, className.c_str(),
                     nullptr, style, left, top, width, height,
                     parentHandle,::CreateMenu(),
                     Application::InstanceHandle(), this);

    assert(windowHandle != nullptr);
    WindowMap[windowHandle] = this;

    ::ShowWindow(windowHandle, windowShow);
    ::RegisterTouchWindow(windowHandle, 0);
  }

  Window::~Window() {
    OnDestroy();

    if (parentPtr != nullptr) {
      parentPtr->SetFocus();
    }

    if (this == Application::MainWindowPtr()) {
      ::PostQuitMessage(0);
    }

    WindowMap.erase(windowHandle);
    ::DestroyWindow(windowHandle);
  }

  void Window::ShowWindow(bool visible) {
    ::ShowWindow(windowHandle, visible ? SW_SHOW : SW_HIDE);
  }

  void Window::EnableWindow(bool enable) {
    ::EnableWindow(windowHandle, enable ? TRUE : FALSE);
  }

  void Window::SetHeader(String headerText) {
    ::SetWindowText(windowHandle, headerText.c_str());
  }

  void Window::SetTimer(int timerId, unsigned int interval) {
    ::SetTimer(windowHandle, timerId, interval, nullptr);
  }

  void Window::DropTimer(int timerId) {
    ::KillTimer(windowHandle, timerId);
  }

  void Window::SetFocus() const {
    ::SetFocus(windowHandle);
  }

  bool Window::HasFocus() const {
    return (::GetFocus() == windowHandle);
  }

  void Window::OnTouchDown(vector<Point> pointList) {
    for (Point touchPoint : pointList) {
      OnMouseDown(NoButton, touchPoint, false, false);
    }
  }

  void Window::OnTouchMove(vector<Point> pointList) {
    for (Point touchPoint : pointList) {
      OnMouseMove(NoButton, touchPoint, false, false);
    }
  }

  void Window::OnTouchUp(vector<Point> pointList) {
    for (Point touchPoint : pointList) {
      OnMouseUp(NoButton, touchPoint, false, false);
    }
  }

  void Window::Invalidate(bool clear /* = true */) const {
    ::InvalidateRect(windowHandle, nullptr, clear ? TRUE : FALSE);
  }

  void Window::Invalidate(Rect areaRect, bool clear /* = true */) const {
    RECT rect = (RECT) LogicalToDevice(areaRect);
    ::InvalidateRect(windowHandle, &rect, clear ? TRUE : FALSE);
  }

  void Window::UpdateWindow() {
    ::UpdateWindow(windowHandle);
  }

  void Window::PrepareDeviceContext(HDC deviceContextHandle) const {
    switch (system) {
      case PreviewCoordinate: {
          RECT clientDeviceRect;
          ::GetClientRect(windowHandle, &clientDeviceRect);

          ::SetMapMode(deviceContextHandle, MM_ANISOTROPIC);
          ::SetWindowExtEx(deviceContextHandle, pageSize.Width(),
                           pageSize.Height(), nullptr);
        ::SetViewportExtEx(deviceContextHandle,
                           clientDeviceRect.right,
                           clientDeviceRect.bottom, nullptr);
        }
        break;

      case LogicalWithScroll:
      case LogicalWithoutScroll:
        ::SetMapMode(deviceContextHandle, MM_ISOTROPIC);

        { int horizontalSize =
                100 * GetDeviceCaps(deviceContextHandle,HORZSIZE),
              verticalSize =
                100 * GetDeviceCaps(deviceContextHandle,VERTSIZE);
          ::SetWindowExtEx(deviceContextHandle, horizontalSize,
                           verticalSize, nullptr);
        }

        { int horizontalResolution = (int) 
               (zoom*GetDeviceCaps(deviceContextHandle, HORZRES)),
             verticalResolution = (int) 
               (zoom*GetDeviceCaps(deviceContextHandle, VERTRES));
         ::SetViewportExtEx(deviceContextHandle,
               horizontalResolution, verticalResolution, nullptr);
        }

        if (system == LogicalWithScroll) {
          int horizontalScroll =
                ::GetScrollPos(windowHandle, SB_HORZ),
              verticalScroll =
                ::GetScrollPos(windowHandle, SB_VERT);
          ::SetWindowOrgEx(deviceContextHandle, horizontalScroll,
                           verticalScroll, nullptr);
        }
        break;
    }
  }

  Point Window::DeviceToLogical(Point point) const {
    HDC deviceContextHandle = ::GetDC(windowHandle);
    PrepareDeviceContext(deviceContextHandle);
    POINT pointStruct = (POINT)point;
    ::DPtoLP(deviceContextHandle, &pointStruct, 1);
    ::ReleaseDC(windowHandle, deviceContextHandle);
    return Point(pointStruct);
  }

  Rect Window::DeviceToLogical(Rect rect) const {
    return Rect(DeviceToLogical(rect.TopLeft()),
                DeviceToLogical(rect.BottomRight()));
  }

  Size Window::DeviceToLogical(Size size) const {
    return ((Size) DeviceToLogical(Rect(ZeroPoint, size)));
  }

  Point Window::LogicalToDevice(Point point) const {
    HDC deviceContextHandle = ::GetDC(windowHandle);
    PrepareDeviceContext(deviceContextHandle);
    POINT pointStruct = (POINT) point;
    ::LPtoDP(deviceContextHandle, &pointStruct, 1);
    ::ReleaseDC(windowHandle, deviceContextHandle);
    return Point(pointStruct);
  }

  Rect Window::LogicalToDevice(Rect rect) const {
    return Rect(LogicalToDevice(rect.TopLeft()),
                LogicalToDevice(rect.BottomRight()));
  }

  Size Window::LogicalToDevice(Size size) const {
    return ((Size) LogicalToDevice(Rect(ZeroPoint, size)));
  }

  Point Window::GetWindowDevicePosition() const {
    return GetWindowDeviceRect().TopLeft();
  }

  void Window::SetWindowDevicePosition(Point topLeft) {
    ::SetWindowPos(windowHandle, nullptr, topLeft.X(), topLeft.Y(),0,0,SWP_NOSIZE);
  }

  Size Window::GetWindowDeviceSize() const {
    return GetWindowDeviceRect().GetSize();
  }

  void Window::SetWindowDeviceSize(Size windowSize) {
    ::SetWindowPos(windowHandle, nullptr, 0, 0,
               windowSize.Width(),windowSize.Height(),SWP_NOMOVE);
  }

  Size Window::GetClientDeviceSize() const {
    RECT rectStruct;
    ::GetClientRect(windowHandle, &rectStruct);
    return Size(rectStruct.right, rectStruct.bottom);
  }

  Rect Window::GetWindowDeviceRect() const {
    RECT windowRect;
    ::GetWindowRect(windowHandle, &windowRect);
    POINT topLeft = {windowRect.left, windowRect.top},
          bottomRight = {windowRect.right, windowRect.bottom};

    if (parentPtr != nullptr) {
      ::ScreenToClient(parentPtr->windowHandle, &topLeft);
      ::ScreenToClient(parentPtr->windowHandle, &bottomRight);
    }

    return Rect(Point(topLeft), Point(bottomRight));
  }

  void Window::SetWindowDeviceRect(Rect windowRect) {
    SetWindowDevicePosition(windowRect.TopLeft());
    SetWindowDeviceSize(windowRect.GetSize());
  }

  Point Window::GetWindowPosition() const {
    return DeviceToLogical(GetWindowDevicePosition());
  }

  void Window::SetWindowPosition(Point topLeft) {
    SetWindowDevicePosition(LogicalToDevice(topLeft));
  }

  Size Window::GetWindowSize() const {
    return DeviceToLogical(GetWindowDeviceSize());
  }

  void Window::SetWindowSize(Size windowSize) {
    SetWindowDeviceSize(LogicalToDevice(windowSize));
  }

  Size Window::GetClientSize() const {
    return DeviceToLogical(GetClientDeviceSize());
  }

  Rect Window::GetWindowRect() const {
    return DeviceToLogical(GetWindowDeviceRect());
  }

  void Window::SetWindowRect(Rect windowRect) {
    SetWindowDeviceRect(LogicalToDevice(windowRect));
  }

  TEXTMETRIC Window::CreateTextMetric(Font font) const {
    font.PointsToLogical();

    HDC deviceContextHandle = ::GetDC(windowHandle);
    PrepareDeviceContext(deviceContextHandle);

    HFONT fontHandle = ::CreateFontIndirect(&font.LogFont());
    HFONT oldFontHandle =
      (HFONT) ::SelectObject(deviceContextHandle, fontHandle);

    TEXTMETRIC textMetric;
    ::GetTextMetrics(deviceContextHandle, &textMetric);

    ::SelectObject(deviceContextHandle, oldFontHandle);
    ::DeleteObject(fontHandle);
    ::ReleaseDC(windowHandle, deviceContextHandle);

    return textMetric;
  }

  int Window::GetCharacterHeight(Font font) const {
    return CreateTextMetric(font).tmHeight;
  }

  int Window::GetCharacterAscent(Font font) const {
    return CreateTextMetric(font).tmAscent;
  }

  int Window::GetCharacterAverageWidth(Font font) const {
    return CreateTextMetric(font).tmAveCharWidth;
  }

  int Window::GetCharacterWidth(Font font, TCHAR tChar) const {
    font.PointsToLogical();

    HDC deviceContextHandle = ::GetDC(windowHandle);
    PrepareDeviceContext(deviceContextHandle);

    HFONT fontHandle = ::CreateFontIndirect(&font.LogFont());
    HFONT oldFontHandle =
      (HFONT) ::SelectObject(deviceContextHandle, fontHandle);

    SIZE szChar;
    ::GetTextExtentPoint(deviceContextHandle, &tChar, 1, &szChar);

    ::SelectObject(deviceContextHandle, oldFontHandle);
    ::DeleteObject(fontHandle);
    ::ReleaseDC(windowHandle, deviceContextHandle);

    return szChar.cx;
  }

  void Window::OnClose() {
    if (TryClose()) {
      delete this;
    }
  }

  Answer Window::MessageBox(String message,
                            String caption /*=TEXT("Error")*/,
                            ButtonGroup buttonGroup /* = Ok */,
                            Icon icon /* = NoIcon */,
                            bool help /* = false */) const {
    return (Answer) ::MessageBox(windowHandle, message.c_str(),
                                 caption.c_str(), buttonGroup |
                                 icon | (help ? MB_HELP : 0));
  }

  void OnTouch(Window* windowPtr, WPARAM wordParam,
               LPARAM longParam, Point windowTopLeft) {
    UINT inputs = LOWORD(wordParam);
    HTOUCHINPUT touchInputHandle = (HTOUCHINPUT) longParam;

    TOUCHINPUT* inputArray = new TOUCHINPUT[inputs];
    assert(inputArray != nullptr);

    if (::GetTouchInputInfo(touchInputHandle, inputs,
                            inputArray, sizeof(TOUCHINPUT))){
      vector<Point> pointList;

      for (UINT index = 0; index < inputs; ++index) {
        Point touchPoint
          ((inputArray[index].x / 100) - windowTopLeft.X(),
           (inputArray[index].y / 100) - windowTopLeft.Y());
        pointList.push_back(touchPoint);
      }

      static DWORD touchId = -1;
      if (touchId != inputArray[0].dwID) {
        touchId = inputArray[0].dwID;
        windowPtr->OnTouchDown(pointList);
      }
      else {
        windowPtr->OnTouchMove(pointList);
      }

      ::CloseTouchInputHandle(touchInputHandle);
    }

    delete [] inputArray;
  }

  LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message,
                              WPARAM wordParam, LPARAM longParam){
    if (WindowMap.count(windowHandle) == 1) {
      Window* windowPtr = WindowMap[windowHandle];

      switch (message) {
        case WM_SETFOCUS:
          windowPtr->OnGainFocus();
          return 0;

        case WM_KILLFOCUS:
          windowPtr->OnLoseFocus();
          return 0;
        
        case WM_TIMER:
          windowPtr->OnTimer((int) wordParam);
          return 0;

        case WM_MOVE: {
            Point windowTopLeft =
              {LOWORD(longParam), HIWORD(longParam)};
            windowPtr->OnMove
                     (windowPtr->DeviceToLogical(windowTopLeft));
          }
          return 0;

        case WM_SIZE: {
            Size clientSize =
              {LOWORD(longParam), HIWORD(longParam)};
            windowPtr->
              OnSize(windowPtr->DeviceToLogical(clientSize));
          }
          return 0;

        case WM_HELP:
          windowPtr->OnHelp();
          break;

        case WM_KEYDOWN: {
            WORD key = wordParam;
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            
            if (windowPtr->OnKeyDown(wordParam, shiftPressed,
                                     controlPressed)) {
              return 0;
            }
          }
          break;

        case WM_CHAR: {
            int asciiCode = (int) wordParam;
            
            if ((asciiCode >= 32) && (asciiCode <= 127)) {
              windowPtr->OnChar((TCHAR) asciiCode);
              return 0;
            }
          }
          break;

        case WM_KEYUP: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);

            if (windowPtr->OnKeyUp(wordParam, shiftPressed,
                                   controlPressed)) {
              return 0;
            }
          }
          break;

        case WM_LBUTTONDOWN: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            ::SetCapture(windowPtr->windowHandle);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnMouseDown(LeftButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
          }
          return 0;

        case WM_MBUTTONDOWN: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            ::SetCapture(windowPtr->windowHandle);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnMouseDown(MiddleButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
          }
          return 0;

        case WM_RBUTTONDOWN: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            ::SetCapture(windowPtr->windowHandle);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnMouseDown(RightButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
          }
          return 0;

        case WM_MOUSEMOVE: {
            MouseButton buttonMask = (MouseButton)
              (((wordParam & MK_LBUTTON) ? LeftButton : 0) |
               ((wordParam & MK_MBUTTON) ? MiddleButton : 0) |
               ((wordParam & MK_RBUTTON) ? RightButton : 0));

            if (buttonMask != NoButton) {
              bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
              bool controlPressed = (::GetKeyState(VK_CONTROL)<0);
              Point mousePoint =
                Point({LOWORD(longParam), HIWORD(longParam)});
              windowPtr->OnMouseMove(buttonMask,
                           windowPtr->DeviceToLogical(mousePoint),
 	                        shiftPressed, controlPressed);
            }
          }
          return 0;

        case WM_LBUTTONUP: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnMouseUp(LeftButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
            ::ReleaseCapture();
          }
          return 0;

        case WM_MBUTTONUP: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            Point mousePoint = Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnMouseUp(MiddleButton, windowPtr->DeviceToLogical(mousePoint),
                                 shiftPressed, controlPressed);
            ::ReleaseCapture();
          }
          return 0;

        case WM_RBUTTONUP: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnMouseUp(MiddleButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
            ::ReleaseCapture();
          }
          return 0;

        case WM_LBUTTONDBLCLK: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnDoubleClick(LeftButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
            ::ReleaseCapture();
          }
          return 0;

        case WM_MBUTTONDBLCLK: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnDoubleClick(MiddleButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
          }
          return 0;

        case WM_RBUTTONDBLCLK: {
            bool shiftPressed = (::GetKeyState(VK_SHIFT) < 0);
            bool controlPressed = (::GetKeyState(VK_CONTROL) < 0);
            Point mousePoint =
              Point({LOWORD(longParam), HIWORD(longParam)});
            windowPtr->OnDoubleClick(RightButton,
                         windowPtr->DeviceToLogical(mousePoint),
                         shiftPressed, controlPressed);
          }
          return 0;

        case WM_TOUCH:
          OnTouch(windowPtr, wordParam, longParam,
                  windowPtr->GetWindowDevicePosition());
          return 0;

        case WM_PAINT: {
            PAINTSTRUCT paintStruct;
            HDC deviceContextHandle =
              ::BeginPaint(windowHandle, &paintStruct);
            windowPtr->PrepareDeviceContext(deviceContextHandle);
            Graphics graphics(windowPtr, deviceContextHandle);
            windowPtr->OnPaint(graphics);
            ::EndPaint(windowHandle, &paintStruct);
          }
          return 0;

        case WM_CLOSE:
          windowPtr->OnClose();
          return 0;

      }
    }

    return DefWindowProc(windowHandle, message, wordParam, longParam);
  }
};
