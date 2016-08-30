#include "..\\SmallWindows.h"

namespace SmallWindows {
  PushButton::PushButton(Dialog* parentPtr, Point topLeft,
                         Size controlSize, String text,
                         VoidListener clickListener,
                         bool default /* = false */)
   :ButtonControl(parentPtr, topLeft, controlSize, text,
                  WS_BORDER | WS_GROUP| WS_TABSTOP |
                  (default ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON)),
    clickListener(clickListener) {
    // Empty.
  }

  void PushButton::OnClick(Dialog* dialogPtr) {
    clickListener(dialogPtr);
  }
};