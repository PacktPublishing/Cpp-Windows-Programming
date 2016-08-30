namespace SmallWindows {
  enum EditStyle {LeftEdit = ES_LEFT, CenterEdit = ES_CENTER,
                  RightEdit = ES_RIGHT, DigitsOnly = ES_NUMBER,
                  ReadOnly = ES_READONLY, Password = ES_PASSWORD,
                  Uppercase = ES_UPPERCASE,Lowercase=ES_LOWERCASE,
                  AutoScroll = ES_AUTOHSCROLL};

  enum {oct = 8, dec = 10, hex = 16};

  template <class Type = String, int Base = dec,
            class TheConverter = Converter<Type>>
  class TextField : public Control {
    public:
      TextField(Dialog* parentPtr, Point topLeft,
                Size controlSize, Type* valuePtr,
                int size = 100, EditStyle style = AutoScroll);

      void OnControlInit(Dialog* dialogPtr);
      void OnLoseFocus(Dialog* dialogPtr);

    protected:
      String GetText() const;
      void SetText(String text);

    private:
      Type* valuePtr;
  };

  template <class Type = String, int Base = dec,
            class TheConverter = Converter<Type>>
  TextField<Type,Base,TheConverter>::TextField
    (Dialog* parentPtr, Point topLeft, Size controlSize,
     Type* valuePtr, int size /* = 100 */,
     EditStyle style /* = AutoScroll */)
   :Control(parentPtr, topLeft, controlSize, TEXT("edit"),
            TEXT(""), style | WS_BORDER | WS_GROUP | WS_TABSTOP),
    valuePtr(valuePtr) {
    // Empty.
  }

  template <class Type = String, int Base = dec,
            class TheConverter = Converter<Type>>
  String TextField<Type,Base,TheConverter>::GetText() const {
    TCHAR buffer[MAX_PATH];
    ::GetWindowText(windowHandle, buffer, MAX_PATH);
    return String(buffer);
  }

  template <class Type = String, int Base = dec,
            class TheConverter = Converter<Type>>
  void TextField<Type,Base,TheConverter>::SetText(String text) {
    ::SetWindowText(windowHandle, text.c_str());
  }

  template <class Type = String, int Base = dec,
            class TheConverter = Converter<Type>>
  void TextField<Type,Base,TheConverter>::OnControlInit
                                          (Dialog* dialogPtr) {
    SetText(TheConverter::ValueToText(*valuePtr, Base));
  }

  template <class Type = String, int Base = dec,
            class TheConverter = Converter<Type>>
  void TextField<Type,Base,TheConverter>::OnLoseFocus
                                          (Dialog* dialogPtr) {
    String text = GetText();

    if (TheConverter::Check(text, Base)) {
      *valuePtr = TheConverter::TextToValue(text, Base);
    }

    SetText(TheConverter::ValueToText(*valuePtr, Base));
  }
};