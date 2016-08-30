#include "SmallWindows.h"

namespace SmallWindows {
  bool StandardDialog::SaveDialog(Window* windowPtr, String& path,
                              const TCHAR* filter,
                              const vector<String> fileSuffixList, 
                              SaveFlags saveFlags
                                /* = NormalSaveFlags */) {
    OPENFILENAME saveFileName;
    memset(&saveFileName, 0, sizeof saveFileName);

    TCHAR pathBuffer[MAX_PATH];
    wcscpy_s(pathBuffer, MAX_PATH, path.c_str());

    saveFileName.lStructSize = sizeof saveFileName;
    saveFileName.hwndOwner = windowPtr->WindowHandle();
    saveFileName.hInstance = Application::InstanceHandle();
    saveFileName.lpstrFilter = filter;
    saveFileName.lpstrFile = pathBuffer;
    saveFileName.nMaxFile = MAX_PATH;
    saveFileName.Flags = saveFlags;

    if (!fileSuffixList.empty()) {
      saveFileName.lpstrDefExt = fileSuffixList.front().c_str();
    }
    else {
      saveFileName.lpstrDefExt = nullptr;
    }

    while (true) {
      if (::GetSaveFileName(&saveFileName) != 0) {
        path = pathBuffer;

        for (String fileWithSuffix : fileSuffixList) {
          if (EndsWith(path, TEXT(".") + fileWithSuffix)) {
            return true;
          }
        }

        windowPtr->MessageBox(TEXT("Undefined file suffix."));
      }
      else {
        return false;
      }
    }
  }

  bool StandardDialog::OpenDialog(Window* windowPtr, String& path,
                              const TCHAR* filter,
                              const vector<String> fileSuffixList, 
                              StandardDialog::OpenFlags openFlags
                                /*=NormalOpenFlags */){
    OPENFILENAME openFileName;
    memset(&openFileName, 0, sizeof openFileName);

    TCHAR pathBuffer[MAX_PATH];
    wcscpy_s(pathBuffer, MAX_PATH, path.c_str());

    openFileName.lStructSize = sizeof openFileName;
    openFileName.hwndOwner = windowPtr->WindowHandle();
    openFileName.hInstance = Application::InstanceHandle();
    openFileName.lpstrFilter = filter;
    openFileName.lpstrFile = pathBuffer;
    openFileName.nMaxFile = MAX_PATH;
    openFileName.Flags = openFlags;

    if (!fileSuffixList.empty()) {
      openFileName.lpstrDefExt = fileSuffixList.front().c_str();
    }
    else {
      openFileName.lpstrDefExt = nullptr;
    }

    while (true) {
      if (::GetOpenFileName(&openFileName) != 0) {
        path = pathBuffer;

        for (String fileWithSuffix : fileSuffixList) {
          if (EndsWith(path, TEXT(".") + fileWithSuffix)) {
            return true;
          }
        }

        windowPtr->MessageBox(TEXT("Undefined file suffix."));
      }
      else {
        return false;
      }
    }
  }

  COLORREF StandardDialog::customColorArray[16];

  bool StandardDialog::ColorDialog(Window* windowPtr,
                                   Color& color) {
    CHOOSECOLOR chooseColor;
    chooseColor.lStructSize = sizeof chooseColor;
    chooseColor.hwndOwner = windowPtr->WindowHandle();
    chooseColor.hInstance = nullptr;
    chooseColor.rgbResult = color.ColorRef();
    chooseColor.lpCustColors = customColorArray;
    chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
    chooseColor.lCustData = 0;
    chooseColor.lpfnHook = nullptr;
    chooseColor.lpTemplateName = nullptr;

    if (::ChooseColor(&chooseColor) != 0) {
      color.ColorRef() = chooseColor.rgbResult;
      return true;
    }

    return false;
  }

  bool StandardDialog::FontDialog(Window* windowPtr, Font& font) {
    LOGFONT logFont = font.LogFont();

    CHOOSEFONT chooseFont;
    memset(&chooseFont, 0, sizeof chooseFont);

    chooseFont.lStructSize = sizeof(CHOOSEFONT);
    chooseFont.hInstance = Application::InstanceHandle();
    chooseFont.hwndOwner = windowPtr->WindowHandle();
    chooseFont.Flags = CF_INITTOLOGFONTSTRUCT |
                       CF_SCREENFONTS | CF_EFFECTS;
    chooseFont.lpLogFont = &logFont;
    chooseFont.rgbColors = font.FontColor().ColorRef();

    if (::ChooseFont(&chooseFont) != 0) {
      font.LogFont() = logFont;
      font.FontColor() = Color(chooseFont.rgbColors);
      return true;
    }

    return false;
  }

  Graphics* StandardDialog::PrintDialog(Window* parentPtr,
                            int totalPages,
                            int& firstPage, int& lastPage,
                            int& copies, bool& sorted) {
    PRINTDLG printDialog;
    memset(&printDialog, 0, sizeof printDialog);
    
    printDialog.lStructSize = sizeof printDialog;
    printDialog.hwndOwner = parentPtr->WindowHandle();
    printDialog.hDevMode = nullptr;
    printDialog.hDevNames = nullptr;
    printDialog.hDC = nullptr;
    printDialog.Flags = PD_ALLPAGES | PD_COLLATE |
                        PD_RETURNDC | PD_NOSELECTION;
    printDialog.nFromPage = 1;
    printDialog.nToPage = totalPages;
    printDialog.nMinPage = 1;
    printDialog.nMaxPage = totalPages;
    printDialog.nCopies = 1;
    printDialog.hInstance = nullptr;
    printDialog.lCustData = 0L;
    printDialog.lpfnPrintHook = nullptr;
    printDialog.lpfnSetupHook = nullptr;
    printDialog.lpPrintTemplateName = nullptr;
    printDialog.lpSetupTemplateName = nullptr;
    printDialog.hPrintTemplate = nullptr;
    printDialog.hSetupTemplate = nullptr;

    if (::PrintDlg(&printDialog) != 0) {
      bool pageIntervalSelected =
        ((printDialog.Flags & PD_SELECTION) != 0);

      if (pageIntervalSelected) {
        firstPage = printDialog.nFromPage;
        lastPage = printDialog.nToPage;
      }
      else {
        firstPage = printDialog.nMinPage;
        lastPage = printDialog.nMaxPage;
      }

      copies = printDialog.nCopies;
      sorted = (printDialog.Flags & PD_COLLATE) != 0;

      return (new Graphics(parentPtr, printDialog.hDC));
    }

    return nullptr;
  }
};
