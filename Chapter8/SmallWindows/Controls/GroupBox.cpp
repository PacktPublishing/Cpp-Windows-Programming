#include "..\\SmallWindows.h"

namespace SmallWindows {
  GroupBox::GroupBox(Dialog* parentPtr, Point topLeft,
                     Size controlSize, String text)
   :ButtonControl(parentPtr, topLeft, controlSize,
                  text, BS_GROUPBOX) {
    // Empty.
  }
};
