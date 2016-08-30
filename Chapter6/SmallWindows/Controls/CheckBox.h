namespace SmallWindows {
  class CheckBox : public ButtonControl {
    public:
      CheckBox(Dialog* parentPtr, Point topLeft,
               Size controlSize, String text, bool* checkPtr);
      
    private:
      void OnControlInit(Dialog* dialogPtr);
      void OnClick(Dialog* dialogPtr);
      bool* checkPtr;
  };
};