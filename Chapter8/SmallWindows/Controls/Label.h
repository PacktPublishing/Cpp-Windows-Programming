namespace SmallWindows {
  class Label : public Control {
    public:
      Label(Dialog* parentPtr, Point topLeft,
            Size controlSize, String text);
  };
};