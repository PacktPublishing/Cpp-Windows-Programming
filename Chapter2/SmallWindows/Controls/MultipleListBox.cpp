#include "..\\SmallWindows.h"

namespace SmallWindows {
  MultipleListBox::MultipleListBox(Dialog* parentPtr,
              Point topLeft, Size controlSize,
              initializer_list<String> textList,
              set<int>* indexSetPtr)
   :ListControl(parentPtr, topLeft, controlSize, LBS_MULTIPLESEL |
                WS_VSCROLL | WS_BORDER | LBS_NOTIFY | WS_GROUP |
                WS_TABSTOP, textList),
    indexSetPtr(indexSetPtr) {
    // Empty.
  }

  void MultipleListBox::OnControlInit(Dialog* dialogPtr) {
    LoadList();
    SelectMultiple(*indexSetPtr);
  }

  void MultipleListBox::OnSelect(Dialog* dialogPtr) {
    *indexSetPtr = GetSelectionMultiple();
  }

  void MultipleListBox::SelectMultiple(set<int>& indexSet) const {
    int size = ::SendMessage(windowHandle, LB_GETCOUNT, 0, 0);

    for (int index = 0; index < size; ++index) {
      BOOL selected = (indexSet.count(index) > 0) ? TRUE : FALSE;
      ::SendMessage(windowHandle, LB_SETSEL, selected, index);
    }
  }

  set<int> MultipleListBox::GetSelectionMultiple() const {
    int size = ::SendMessage(windowHandle, LB_GETCOUNT, 0, 0);

    set<int> indexSet;
    for (int index = 0; index < size; ++index) {
      if (::SendMessage(windowHandle, LB_GETSEL, index, 0) != 0) {
        indexSet.insert(index);
      }
    }

    return indexSet;
  }
};
