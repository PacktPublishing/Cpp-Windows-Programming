#include "SmallWindows.h"

namespace SmallWindows {
  Menu::Menu(Document* documentPtr, String text /* = TEXT("") */)
   :documentPtr(documentPtr),
    menuText(text),
    menuHandle(::CreateMenu()) {
    // Empty.
  }

  Menu::Menu(const Menu& menu)
   :documentPtr(menu.documentPtr),
    menuText(menu.menuText),
    menuHandle(menu.menuHandle) {
    // Empty.
  }

  void Menu::AddMenu(Menu& menu) {
    ::AppendMenu(menuHandle, MF_STRING | MF_POPUP,
                 (UINT) menu.menuHandle, menu.menuText.c_str());
  }

  void Menu::AddSeparator() {
    ::AppendMenu(menuHandle, MF_SEPARATOR, 0, nullptr);
  }

  void Menu::AddItem(String text, VoidListener selection,
                     BoolListener enable /* = nullptr */,
                     BoolListener check /* = nullptr */,
                     BoolListener radio /* = nullptr */) {
    assert((selection != nullptr) &&
           ((check == nullptr) || (radio == nullptr)));

    map<WORD, Command>& commandMap = documentPtr->CommandMap();
    int itemId = commandMap.size();
    commandMap[itemId] = Command(selection, enable, check, radio);
    ::AppendMenu(menuHandle, MF_STRING, (UINT) itemId, text.c_str());

    if (radio != nullptr) {
      MENUITEMINFO menuItemInfo;
      menuItemInfo.cbSize = sizeof menuItemInfo;
      menuItemInfo.fMask = MIIM_FTYPE;
      menuItemInfo.fType = MFT_RADIOCHECK;
      ::SetMenuItemInfo(menuHandle, (UINT) itemId, FALSE, &menuItemInfo);
    }

    Accelerator::TextToAccelerator(text, itemId,
                                   documentPtr->AcceleratorSet());
  }
};