namespace SmallWindows {
  class ComboBox : public Control {
    public:
      ComboBox(Dialog* parentPtr, Point topLeft, Size controlSize,
               initializer_list<String> textList, int* indexPtr);
      void OnControlInit(Dialog* dialogPtr);
      void OnSelect(Dialog* dialogPtr);

    private:
      void LoadCombo() const;
      void SelectCombo(int index) const;
      int GetComboSelection() const;
      list<String> textList;
      int* indexPtr;
  };
};