#include "..\\SmallWindows.h"

namespace SmallWindows {
  ListBox::ListBox(Dialog* parentPtr, Point topLeft,
             Size controlSize, initializer_list<String>  textList,
             int* indexPtr)
   :ListControl(parentPtr, topLeft, controlSize, WS_VSCROLL |
                WS_BORDER | LBS_NOTIFY | WS_GROUP | WS_TABSTOP,
                textList),
    indexPtr(indexPtr) {
    // Empty.
  }

  void ListBox::OnControlInit(Dialog* dialogPtr) {
    LoadList();
    SelectList(*indexPtr);
  }

  void ListBox::OnSelect(Dialog* dialogPtr) {
    *indexPtr = GetListSelection();
  }

  void ListBox::SelectList(int index) const {
    ::SendMessage(windowHandle, LB_SETCURSEL, index, 0);
  }

  int ListBox::GetListSelection() const {
    return ::SendMessage(windowHandle, LB_GETCURSEL, 0, 0);
  }
};