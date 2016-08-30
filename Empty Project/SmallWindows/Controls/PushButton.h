namespace SmallWindows {
  class PushButton : public ButtonControl {
    public:
      PushButton(Dialog* parentPtr, Point topLeft,
                 Size controlSize, String text,
                 VoidListener clickListener,
                 bool default = false);
      void OnClick(Dialog* dialogPtr);

    private:
      VoidListener clickListener;
  };
};