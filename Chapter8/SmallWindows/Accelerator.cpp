#include "SmallWindows.h"

namespace SmallWindows {
  WORD TextToVirtualKey(String& text) {
    static const struct {
      TCHAR* textPtr;
      WORD key;
    } keyTable[] = {
      {TEXT("F1"), KeyF1}, {TEXT("F2"), KeyF2},
      {TEXT("F3"), KeyF3}, {TEXT("F4"), KeyF4},
      {TEXT("F5"), KeyF5}, {TEXT("F6"), KeyF6},
      {TEXT("F7"), KeyF7}, {TEXT("F8"), KeyF8},
      {TEXT("F9"), KeyF9}, {TEXT("F10"), KeyF10},
      {TEXT("F11"), KeyF11}, {TEXT("F12"), KeyF12},
      {TEXT("Insert"), KeyInsert}, {TEXT("Delete"), KeyDelete},
      {TEXT("Back"), KeyBackspace}, {TEXT("Tab"), KeyTabulator},
      {TEXT("Home"), KeyHome}, {TEXT("End"), KeyEnd},
      {TEXT("Page Up"), KeyPageUp},
      {TEXT("Page Down"), KeyPageDown},
      {TEXT("Left"), KeyLeft}, {TEXT("Right"), KeyRight},
      {TEXT("Up"), KeyUp}, {TEXT("Down"), KeyDown},
      {TEXT("Space"), KeySpace},  {TEXT("Escape"), KeyEscape},
      {TEXT("Return"), KeyReturn}, {nullptr, 0}};

    for (int index = 0; keyTable[index].textPtr != nullptr;
         ++index) {
      if (text == keyTable[index].textPtr) {
        return keyTable[index].key;
      }
    }

    assert(false);
    return 0;
  }

  void Accelerator::TextToAccelerator(String& text, int itemId,
                                      list<ACCEL>&acceleratorSet){
    int tabulatorIndex = text.find(TEXT("\t"));
    if (tabulatorIndex != -1) {
      ACCEL accelerator;
      accelerator.fVirt = 0;
      accelerator.cmd = itemId;
      String acceleratorText = text.substr(tabulatorIndex + 1);

      { String controlText = TEXT("Ctrl+");
        int controlIndex = acceleratorText.find(controlText);

        if (controlIndex != -1) {
          accelerator.fVirt |= FCONTROL;
          acceleratorText.erase(controlIndex,
                                controlText.length());
        }
      }

      { String altText = TEXT("Alt+");
        int altIndex = acceleratorText.find(altText);

        if (altIndex != -1) {
          accelerator.fVirt |= FALT;
          acceleratorText.erase(altIndex, altText.length());
        }
      }

      { String shiftText = TEXT("Shift+");
        int shiftIndex = acceleratorText.find(shiftText);

        if (shiftIndex != -1) {
          accelerator.fVirt |= FSHIFT;
          acceleratorText.erase(shiftIndex, shiftText.length());
        }
      }

      if (acceleratorText.length() == 1) {
        accelerator.key =
          (WORD) ((tolower(acceleratorText[0]) - 'a') + 1);
      }
      else {
        accelerator.fVirt |= FVIRTKEY;
        accelerator.key = TextToVirtualKey(acceleratorText);
      }

      assert(accelerator.fVirt != 0);
      acceleratorSet.push_back(accelerator);
    }

  }
};