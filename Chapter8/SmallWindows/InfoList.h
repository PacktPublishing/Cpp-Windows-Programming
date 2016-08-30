namespace SmallWindows {
  class InfoList {
    public:
      template <class AlignType> void Align();
      template <class ListType>
        void AddValue(const ListType value);
      template <class ListType>
        void PeekValue(ListType& value, int index);
      template <class ListType> void GetValue(ListType& value);
        template <class CharType>
        void AddString(basic_string<CharType> text);
      template <class CharType>
        basic_string<CharType> GetString();
      void FromBuffer(const void* voidBuffer, int size);
      void ToBuffer(void* voidBuffer);
      int Size() const {return list.Size();}

    private:
      DynamicList<char> list;
  };

  template <class AlignType>
  void InfoList::Align() {
    int size = sizeof(AlignType);
    while ((list.Size() % size) > 0) {
      list.PushBack(0);
    }
  }

  template <class ListType>
  void InfoList::AddValue(const ListType value) {
    int size = sizeof(ListType);
    const char* buffer = (char*) &value;

    for (int count = 0; count < size; ++count) {
      list.PushBack(*(buffer++));
    }
  }

  template <class ListType>
  void InfoList::PeekValue(ListType& value, int index) {
    int size = sizeof(ListType);
    char* buffer = (char*) &value;

    for (int count = 0; count < size; ++count) {
      *(buffer++) = list[index + count];
    }
  }

  template <class ListType>
  void InfoList::GetValue(ListType& value) {
    int size = sizeof(ListType);
    char* buffer = (char*) &value;

    for (int count = 0; count < size; ++count) {
      *(buffer++) = list.Front();
      list.Erase(0);
    }
  }

  template <class CharType>
  void InfoList::AddString(basic_string<CharType> text) {
    for (CharType c : text) {
      AddValue<CharType>(c);
    }

    AddValue<CharType>((CharType) 0);
  }

  template <class CharType>
  basic_string<CharType> InfoList::GetString() {
    bacic_string<CharType> text;

    CharType c, zero = (CharType) 0;
    while ((c = GetValue<CharType>()) != zero) {
      text.append(c);
    }

    return text;
  }
};