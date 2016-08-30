#include "..\\SmallWindows.h"

namespace SmallWindows {
  Label::Label(Dialog* parentPtr, Point topLeft,
               Size controlSize, String text)
   :Control(parentPtr, topLeft, controlSize,
            TEXT("static"), text, 0) {
  }
};