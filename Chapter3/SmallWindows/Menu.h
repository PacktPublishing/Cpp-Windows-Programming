#define DEFINE_BOOL_LISTENER(SubClass, Listener)  \
  virtual bool Listener() const;                  \
  static bool SubClass::Listener(void* voidPtr) { \
    return ((SubClass*) voidPtr)->Listener();     \
  }

#define DEFINE_VOID_LISTENER(SubClass, Listener)  \
  virtual void Listener();                        \
  static void SubClass::Listener(void* voidPtr) { \
    ((SubClass*) voidPtr)->Listener();            \
  }

namespace SmallWindows {
  class Document;

  class Menu {
    public:
      Menu(Document* documentPtr, String text = TEXT(""));
      Menu(const Menu& menu);

      void AddMenu(Menu& menu);
      void AddSeparator();
      void AddItem(String text, VoidListener selection,
                   BoolListener enable = nullptr,
                   BoolListener check = nullptr,
                   BoolListener radio = nullptr);

    private:
      Document* documentPtr;
      String menuText;
      HMENU menuHandle;

      friend class Document;
      friend class StandardDocument;
  };
};