namespace SmallWindows {
  class Dialog;

  class Control : public Window {
    public:
      Control(Dialog* parentPtr, Point topLeft, Size controlSize,
              String className, String text, int style);
      void AddControlInfo(InfoList& infoList) const;

      Point TopLeft() const {return topLeft;}
      Size GetSize() const {return controlSize;}

      Rect OriginalRect() const {return originalRect;}
      Rect& OriginalRect() {return originalRect;}

      virtual void OnControlInit(Dialog* dialogPtr) {/* Empty. */ }
      virtual void OnGainFocus(Dialog* dialogPtr) {/* Empty. */ }
      virtual void OnLoseFocus(Dialog* dialogPtr) {/* Empty. */}
      virtual void OnChange(Dialog* dialogPtr) {/* Empty. */}
      virtual void OnSelect(Dialog* dialogPtr)  {/* Empty. */}
      virtual void OnClick(Dialog* dialogPtr) {/* Empty. */}

    private:
      int controlId, style;
      Point topLeft;
      Size controlSize;
      String className;
      String text;
      Rect originalRect;
  };
};