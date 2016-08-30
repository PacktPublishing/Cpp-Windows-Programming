namespace SmallWindows {

  class ButtonControl : public Control {
    public:
      ButtonControl(Dialog* parentPtr, Point topLeft,
                    Size controlSize, String text, int style);

    protected:
      void Check(bool check) const;
      bool IsChecked() const;
  };
};