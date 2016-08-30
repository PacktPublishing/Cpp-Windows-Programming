namespace SmallWindows {
  extern map<HWND,Dialog*> dialogMap;
  extern Font DialogFont;

  class Dialog : public Window {
    public:
      Dialog(String name, Point topLeft,
             Window* parentPtr = nullptr,
             WindowStyle style = OverlappedWindow,
             WindowStyle extendedStyle = NoStyle,
             Font font = DialogFont);
      ~Dialog();
      bool DoModal();

      int AddControl(Control* controlPtr);
      void OnSize(Size windowSize);

      void OnReturn();
      void OnEscape();

      virtual void OnDialogInit() {/* Empty. */}
      virtual bool TryClose() const { return true;}
      virtual void OnClose();
      virtual void OnDestroy() {/* Empty. */}

      map<WORD,Control*> IdMap() const {return idMap;}
      map<WORD,Control*>& IdMap() {return idMap;}

    private:
      String header;
      Point topLeft;
      WindowStyle style;

      WindowStyle extendedStyle;
      Font font;

      map<WORD,Control*> idMap;
      int leftMargin, maxWidth, topMargin, maxHeight;
      int currentId = 1000;
      Size originalClientSize;

      friend INT_PTR CALLBACK
        DialogProc(HWND windowHandle, UINT message,
                   WPARAM wordParam, LPARAM longParam);
  };
};