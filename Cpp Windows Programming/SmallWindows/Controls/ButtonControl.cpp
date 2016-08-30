#include "..\\SmallWindows.h"

namespace SmallWindows {
  ButtonControl::ButtonControl(Dialog* parentPtr, Point topLeft,
                       Size controlSize, String text, int style)
   :Control(parentPtr, topLeft, controlSize,
            TEXT("button"), text, style) {
    // Empty.
  }

  void ButtonControl::Check(bool check) const {
    ::SendMessage(windowHandle, BM_SETCHECK, check ? 1 : 0, 0);
  }

  bool ButtonControl::IsChecked() const {
    return (::SendMessage(windowHandle, BM_GETCHECK, 0, 0) != 0);
  }
};