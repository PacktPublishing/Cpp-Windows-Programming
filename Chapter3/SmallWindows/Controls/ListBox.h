namespace SmallWindows {
  class ListBox : public ListControl {
    public:
      ListBox(Dialog* parentPtr, Point topLeft, Size controlSize,
              initializer_list<String> textList, int* indexPtr);
      void OnControlInit(Dialog* dialogPtr);
      void OnSelect(Dialog* dialogPtr);

    private:
      void SelectList(int index) const;
      int GetListSelection() const;
      int* indexPtr;
  };
};