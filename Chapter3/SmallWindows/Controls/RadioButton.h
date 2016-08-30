namespace SmallWindows {
  class RadioButton : public ButtonControl {
    public:
      RadioButton(Dialog* parentPtr, Point topLeft, Size size,
                  String text, int* indexPtr, int index);
      void OnControlInit(Dialog* dialogPtr);
      void OnClick(Dialog* dialogPtr);

    private:
      int *indexPtr, index;
  };
};