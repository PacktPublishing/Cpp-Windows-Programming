namespace SmallWindows {
  class GroupBox : public ButtonControl {
    public:
      GroupBox(Dialog* parentPtr, Point topLeft,
               Size controlSize, String text);
  };
};