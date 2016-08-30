#include "..\\SmallWindows.h"

namespace SmallWindows {
  CheckBox::CheckBox(Dialog* parentPtr, Point topLeft,
                 Size controlSize, String text, bool* checkPtr)
   :ButtonControl(parentPtr, topLeft, controlSize, text,
                  BS_AUTOCHECKBOX | WS_GROUP | WS_TABSTOP),
    checkPtr(checkPtr) {
  }

  void CheckBox::OnControlInit(Dialog* dialogPtr) {
    Check(*checkPtr);
  }

  void CheckBox::OnClick(Dialog* dialogPtr) {
    *checkPtr = IsChecked();
  }
};