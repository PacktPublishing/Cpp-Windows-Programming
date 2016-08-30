#include "..\\SmallWindows.h"

namespace SmallWindows {
  RadioButton::RadioButton(Dialog* parentPtr, Point topLeft,
                           Size size, String text, int* indexPtr,
                           int index)
   :ButtonControl(parentPtr, topLeft, size, text,
                  BS_AUTORADIOBUTTON |
                  ((index == 0) ? (WS_GROUP | WS_TABSTOP) : 0)),
    indexPtr(indexPtr),
    index(index) {
    // Empty.
  }

  void RadioButton::OnControlInit(Dialog* dialogPtr) {
    Check((*indexPtr) == index);
  }

  void RadioButton::OnClick(Dialog* dialogPtr) {
    *indexPtr = index;
  }
};