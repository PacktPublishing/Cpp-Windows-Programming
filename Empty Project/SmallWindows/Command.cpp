#include "SmallWindows.h"

namespace SmallWindows {
  Command::Command()
   :selection(nullptr),
    enable(nullptr),
    check(nullptr),
    radio(nullptr) {
    // Empty.
  }

  Command::Command(VoidListener selection, BoolListener enable,
                   BoolListener check, BoolListener radio)
   :selection(selection),
    enable(enable),
    check(check),
    radio(radio) {
    // Empty.
  }
};