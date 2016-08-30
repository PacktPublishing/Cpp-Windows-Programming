namespace SmallWindows {
  class ListControl : public Control {
    public:
      ListControl(Dialog* parentPtr, Point topLeft,
                  Size controlSize, int style,
                  list<String> textList);

    protected:
      void LoadList() const;

    private:
      list<String> textList;
  };
};