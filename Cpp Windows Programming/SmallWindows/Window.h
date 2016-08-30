namespace SmallWindows {
  extern map<HWND,Window*> WindowMap;

  enum WindowStyle {NoStyle = 0, Border = WS_BORDER,
                    ThickFrame = WS_THICKFRAME,
                    Caption = WS_CAPTION, Child = WS_CHILD,
                    ClipChildren = WS_CLIPCHILDREN,
                    ClipSibling = WS_CLIPSIBLINGS,
                    Disabled = WS_DISABLED,
                    DialogFrame = WS_DLGFRAME, Group = WS_GROUP,
                    HScroll = WS_HSCROLL, Minimize = WS_MINIMIZE,
                    Maximize = WS_MAXIMIZE,
                    MaximizeBox = WS_MAXIMIZEBOX,
                    MinimizeBox = WS_MINIMIZEBOX,
                    Overlapped = WS_OVERLAPPED,
                    OverlappedWindow = WS_OVERLAPPEDWINDOW,
                    Popup = WS_POPUP,PopupWindow = WS_POPUPWINDOW,
                    SystemMenu = WS_SYSMENU,
                    Tabulatorstop = WS_TABSTOP,
                    Thickframe = WS_THICKFRAME,
                    Tiled = WS_TILED, Visible = WS_VISIBLE,
                    VScroll = WS_VSCROLL};

  enum WindowShow {Restore = SW_RESTORE, Default = SW_SHOWDEFAULT,
                   Maximized = SW_SHOWMAXIMIZED,
                   Minimized = SW_SHOWMINIMIZED,
                   MinNoActive = SW_SHOWMINNOACTIVE,
                   NoActive = SW_SHOWNA,
                   NoActivate = SW_SHOWNOACTIVATE,
                   Normal = SW_SHOWNORMAL,
                   Show = SW_SHOW, Hide = SW_HIDE};

  enum MouseButton {NoButton = 0x00, LeftButton = 0x01,
                    MiddleButton = 0x02, RightButton = 0x04};

  enum WheelDirection {WheelUp, WheelDown};

  enum CoordinateSystem {LogicalWithScroll, LogicalWithoutScroll,
                         PreviewCoordinate};

  enum ButtonGroup {Ok = MB_OK, OkCancel = MB_OKCANCEL,
                    YesNo = MB_YESNO,
                    YesNoCancel = MB_YESNOCANCEL,
                    RetryCancel = MB_RETRYCANCEL,
                    CancelTryContinue = MB_CANCELTRYCONTINUE,
                    AbortRetryIgnore = MB_ABORTRETRYIGNORE};
  
  enum Icon {NoIcon = 0, Information = MB_ICONINFORMATION,
             Stop = MB_ICONSTOP, Warning = MB_ICONWARNING,
             Question = MB_ICONQUESTION};
  
  enum Answer {OkAnswer = IDOK, Cancel = IDCANCEL, Yes = IDYES,
               No = IDNO, Retry = IDRETRY, Continue = IDCONTINUE,
               Abort = IDABORT, Ignore = IDIGNORE} const;

  enum DrawMode {Paint, Print};

  class Application;

  class Window {
    public:
      Window(CoordinateSystem system, Size pageSize = ZeroSize,
             Window* parentPtr = nullptr,
             WindowStyle style = OverlappedWindow,
             WindowStyle extendedStyle = NoStyle,
             WindowShow windowShow = Normal,
             Point topLeft = ZeroPoint, Size windowSize=ZeroSize);

    protected:
      Window(Window* parentPtr = nullptr);
      Window(String className, CoordinateSystem system,
             Size pageSize = ZeroSize,
             Window* parentPtr = nullptr,
             WindowStyle style = OverlappedWindow,
             WindowStyle extendedStyle = NoStyle,
             WindowShow windowShow = Normal,
             Point windowTopLeft = ZeroPoint,
             Size windowSize = ZeroSize);

      void PrepareDeviceContext(HDC deviceContextHandle) const;

    public:
      virtual ~Window();

      void ShowWindow(bool visible);
      void EnableWindow(bool enable);

      virtual void OnSize(Size windowSize) {/* Empty. */}
      virtual void OnMove(Point topLeft) {/* Empty. */ }
      virtual void OnHelp() {/* Empty. */}

      HWND WindowHandle() const {return windowHandle;}
      HWND& WindowHandle() {return windowHandle;}
      Window* ParentWindowPtr() const {return parentPtr;}
      Window*& ParentWindowPtr() {return parentPtr;}
      void SetHeader(String headerText);

      double GetZoom() const {return zoom;}
      void SetZoom(double z) {zoom = z;}

      void SetTimer(int timerId, unsigned int interval);
      void DropTimer(int timerId);
      virtual void OnTimer(int timerId) {/* Empty. */}

      void SetFocus() const;
      bool HasFocus() const;

      virtual void OnGainFocus() {/* Empty. */}
      virtual void OnLoseFocus() {/* Empty. */}

      virtual void OnMouseDown(MouseButton mouseButtons,
                     Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed) {/* Empty. */}
      virtual void OnMouseUp(MouseButton mouseButtons,
                     Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed) {/* Empty. */}
      virtual void OnMouseMove(MouseButton mouseButtons,
                     Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed) {/* Empty. */}

      virtual void OnDoubleClick(MouseButton mouseButtons,
                     Point mousePoint, bool shiftPressed,
                     bool controlPressed) {/* Empty. */}

      virtual void OnMouseWheel(WheelDirection direction,
                          bool shiftPressed,
                          bool controlPressed) {/* Empty. */}

      virtual void OnTouchDown(vector<Point> pointList);
      virtual void OnTouchMove(vector<Point> pointList);
      virtual void OnTouchUp(vector<Point> pointList);

      virtual bool OnKeyDown(WORD key, bool shiftPressed,
                             bool controlPressed) {return false;}
      virtual void OnChar(TCHAR tChar) {/* Empty. */}
      virtual bool OnKeyUp(WORD key, bool shiftPressed,
                           bool controlPressed) {return false;}

      void Invalidate(bool clear = true) const;
      void Invalidate(Rect areaRect, bool clear = true) const;
      void UpdateWindow();

      virtual void OnPaint(Graphics& graphics) const
                          {OnDraw(graphics, Paint);}
      virtual void OnPrint(Graphics& graphics, int page,
                           int copy, int totalPages) const
                           {OnDraw(graphics, Print);}
      virtual void OnDraw(Graphics& graphics,
                          DrawMode drawMode) const {/* Empty. */}

      virtual bool TryClose() {return true;}
      virtual void OnClose();
      virtual void OnDestroy() {/* Empty. */}

    protected:
      Point DeviceToLogical(Point point) const;
      Rect DeviceToLogical(Rect rect) const;
      Size DeviceToLogical(Size size) const;
      Point LogicalToDevice(Point point) const;
      Rect LogicalToDevice(Rect rect) const;
      Size LogicalToDevice(Size size) const;

    public:
      Point GetWindowDevicePosition() const;
      void SetWindowDevicePosition(Point topLeft);
      Size GetWindowDeviceSize() const;
      void SetWindowDeviceSize(Size windowSize);
      Size GetClientDeviceSize() const;
      Rect GetWindowDeviceRect() const;
      void SetWindowDeviceRect(Rect windowRect);

      Size GetWindowSize() const;
      void SetWindowSize(Size windowSize);
      Point GetWindowPosition() const;
      void SetWindowPosition(Point topLeft);
      Size GetClientSize() const;
      Rect GetWindowRect() const;
      void SetWindowRect(Rect windowRect) ;

    private:
      TEXTMETRIC CreateTextMetric(Font font) const;

    public:
      int GetCharacterAverageWidth(Font font) const;
      int GetCharacterHeight(Font font) const;
      int GetCharacterAscent(Font font) const;
      int GetCharacterWidth(Font font, TCHAR tChar) const;

      Answer MessageBox(String message,
                    String caption = TEXT("Error"),
                    ButtonGroup buttonGroup = Ok,
                    Icon icon = NoIcon, bool help = false) const;

    protected:
      const Size pageSize;
      HWND windowHandle;
      Window* parentPtr;

    private:
      CoordinateSystem system;
      double zoom = 1.0;

      friend LRESULT CALLBACK WindowProc(HWND windowHandle,
                                 UINT message, WPARAM wordParam,
                                 LPARAM longParam);
  };

  extern map<HWND,Window*> WindowMap;
};