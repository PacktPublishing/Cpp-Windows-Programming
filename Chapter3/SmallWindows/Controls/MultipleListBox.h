namespace SmallWindows {
  class MultipleListBox : public ListControl {
    public:
      MultipleListBox(Dialog* parentPtr, Point topLeft,
              Size controlSize, initializer_list<String> textList,
              set<int>* indexSetPtr);
      void OnControlInit(Dialog* dialogPtr);
      void OnSelect(Dialog* dialogPtr);

    private:
      void SelectMultiple(set<int>& indexSet) const;
      set<int> GetSelectionMultiple() const;
      set<int>* indexSetPtr;
  };
};