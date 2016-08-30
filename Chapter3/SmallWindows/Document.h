namespace SmallWindows {
  extern const Size USLetterPortrait, LineSize;

  enum KeyboardMode {InsertKeyboard, OverwriteKeyboard};

  class Document : public Window {
    public:
      Document(CoordinateSystem system, Size pageSize,
               Window* parentPtr = nullptr,
               WindowStyle style=OverlappedWindow,
               WindowShow windowShow = Normal,
               bool acceptDropFiles = true,
               Size lineSize = LineSize);

    protected:
      Document(String className, CoordinateSystem system,
               Size pageSize, Window* parentPtr = nullptr,
               WindowStyle style = OverlappedWindow,
               WindowShow windowShow = Normal,
               bool acceptDropFiles = true,
               Size lineSize = LineSize);

    public:
      ~Document();

      String GetName() const;
      void SetName(String name);
      void SetZoom(double zoom);
      bool IsDirty() const;
      void SetDirty(bool dirty);
      
    private:
      void GenerateHeader(); 

    public:
      virtual void OnSize(Size clientSize);

      virtual void OnMouseWheel(WheelDirection direction,
                          bool shiftPressed, bool controlPressed);

      void SetCaret(Rect caretRect);
      void ClearCaret();
      virtual void UpdateCaret() {/* Empty. */ }

      void OnGainFocus();
      void OnLoseFocus();

      void SetMenuBar(Menu& menuBar);
      void OnCommand(WORD commandId);
      void OnCommandInit();

      virtual void OnDropFile(vector<String> pathList)
                             {/* Empty. */}

      KeyboardMode GetKeyboardMode() const {return keyboardMode;}
      void SetKeyboardMode(KeyboardMode mode)
                          {keyboardMode = mode;}
      virtual void OnKeyboardMode(KeyboardMode mode)
                                 {/* Empty. */}

      virtual void OnHorizontalScroll(WORD flags,WORD thumbPos=0);
      virtual void OnVerticalScroll(WORD flags, WORD thumbPos =0);

      virtual bool KeyToScroll(WORD key, bool shiftPressed,
                               bool controlPressed);

      void SetHorizontalScrollPosition(int scrollPos);
      int GetHorizontalScrollPosition() const;
      void SetVerticalScrollPosition(int scrollPos);
      int GetVerticalScrollPosition() const;

      void SetHorizontalScrollLineWidth(int lineWidth);
      int GetHorizontalScrollLineWidth() const;
      void SetVerticalScrollLineHeight(int lineHeight);
      int GetVerticalScrollLineHeight() const;

      void SetHorizontalScrollPageWidth(int pageWidth);
      int GetHorizontalScrollPageWidth() const;
      void SetVerticalScrollPageHeight(int pageHeight);
      int GetVerticalScrollPageHeight() const;

      void SetHorizontalScrollTotalWidth(int scrollWidth);
      int GetHorizontalScrollTotalWidth() const;
      void SetVerticalScrollTotalHeight(int scrollHeight);
      int GetVerticalScrollTotalHeight() const;

    public:
      map<WORD,Command>& CommandMap() {return commandMap;}
      list<ACCEL>& AcceleratorSet() {return acceleratorSet;}

    private:
      map<WORD, Command> commandMap;
      list<ACCEL> acceleratorSet;
      String name;
      bool caretPresent = false;
      Size lineSize;
      bool dirtyFlag = false;
      HMENU menuBarHandle;
      KeyboardMode keyboardMode = InsertKeyboard;
  };

  LRESULT CALLBACK DocumentProc(HWND windowHandle, UINT message,
                           WPARAM wordParam,LPARAM longParam);
  vector<String> ExtractPathList(WORD wordParam);
};