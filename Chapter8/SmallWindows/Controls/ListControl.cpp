#include "..\\SmallWindows.h"

namespace SmallWindows {
  ListControl::ListControl(Dialog* parentPtr, Point topLeft,
                           Size controlSize, int style,
                           list<String> textList)
   :Control(parentPtr, topLeft, controlSize,
            TEXT("listbox"), TEXT(""), style),
    textList(textList) {
    // Empty.
  }

  void ListControl::LoadList() const {
    for (String text : textList) {
      ::SendMessage(windowHandle, LB_ADDSTRING,
                    0, (LPARAM) text.c_str());
    }
  }
};
