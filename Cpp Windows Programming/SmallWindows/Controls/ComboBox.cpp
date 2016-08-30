#include "..\\SmallWindows.h"

namespace SmallWindows {
  ComboBox::ComboBox(Dialog* parentPtr, Point topLeft,
            Size controlSize, initializer_list<String> textList,
            int* indexPtr)
   :Control(parentPtr, topLeft, controlSize, TEXT("combobox"),
            TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | LBS_NOTIFY |
            LBS_COMBOBOX | WS_GROUP | WS_TABSTOP),
    textList(textList),
    indexPtr(indexPtr) {
    // Empty.
  }

  void ComboBox::OnControlInit(Dialog* dialogPtr) {
    LoadCombo();
    SelectCombo(*indexPtr);
  }

  void ComboBox::OnSelect(Dialog* dialogPtr) {
    *indexPtr = GetComboSelection();
  }

  void ComboBox::LoadCombo() const {
    for (String text : textList) {
      ::SendMessage(windowHandle, CB_ADDSTRING,
                    0, (LPARAM) text.c_str());
    }
  }

  void ComboBox::SelectCombo(int index) const {
    ::SendMessage(windowHandle, CB_SETCURSEL, index, 0);
  }

  int ComboBox::GetComboSelection() const {
    return ::SendMessage(windowHandle, CB_GETCURSEL, 0, 0);
  }
};
