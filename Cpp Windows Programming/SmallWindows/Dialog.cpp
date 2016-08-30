#include "SmallWindows.h"

namespace SmallWindows {
  map<HWND,Dialog*> dialogMap;
  Font DialogFont(TEXT("Times New Roman"), 12);

  Dialog::Dialog(String header, Point topLeft,
                 Window* parentPtr /*=nullptr*/,
                 WindowStyle style /* = OverlappedWindow */,
                 WindowStyle extendedStyle /* = NoStyle */,
                 Font font /* = DialogFont */)
   :Window(parentPtr),
    header(header),
    topLeft(topLeft),
    style(style),
    extendedStyle(extendedStyle),
    font(font) {
    // Empty.
  }

  bool Dialog::DoModal() {
    InfoList infoList;
    infoList.AddValue<WORD>(1); // Dialog version
    infoList.AddValue<WORD>(0xFFFF); // Extended dialog template
    infoList.AddValue<DWORD>(0); // Help ID
    infoList.AddValue<DWORD>(extendedStyle);
    infoList.AddValue<DWORD>(style | WS_CAPTION | WS_SYSMENU |
                             DS_MODALFRAME | DS_SETFONT);
    infoList.AddValue<WORD>(idMap.size()); // Number of controls
    infoList.AddValue<WORD>(topLeft.X());
    infoList.AddValue<WORD>(topLeft.Y());
    infoList.AddValue<WORD>(maxWidth + leftMargin); // Width
    infoList.AddValue<WORD>(maxHeight + topMargin); // Height
    infoList.AddValue<WORD>(0); // No menu
    infoList.AddValue<WORD>(0); // Default dialog class
    infoList.AddString<TCHAR>(header); // Title

    LOGFONT logFont = font.LogFont();
    infoList.AddValue<WORD>((WORD) logFont.lfHeight);
    infoList.AddValue<WORD>((WORD) logFont.lfWeight);
    infoList.AddValue<BYTE>(logFont.lfItalic);
    infoList.AddValue<BYTE>(logFont.lfCharSet);
    infoList.AddString<TCHAR>(logFont.lfFaceName);

    for (pair<WORD,Control*> entry : idMap) {
      Control* controlPtr = entry.second;
      controlPtr->AddControlInfo (infoList);
    }

/*
    list<Control*> controlPtrList;
    for (pair<WORD, Control*> entry : idMap) {
      Control* controlPtr = entry.second;
      controlPtrList.push_front(controlPtr);
    }

    for (const Control* controlPtr : controlPtrList) {
      controlPtr->AddControlInfo(infoList);
    }
*/

    HGLOBAL globalHandle = ::GlobalAlloc(0, infoList.Size());
    if (globalHandle != nullptr) {
      char* buffer = (char*) ::GlobalLock(globalHandle);

      if (buffer != nullptr) {
        infoList.ToBuffer(buffer);
        HWND parentHandle = (parentPtr != nullptr) ?
                            parentPtr->WindowHandle() : nullptr;
        INT_PTR result =
          ::DialogBoxIndirectParam(Application::InstanceHandle(),
                     (DLGTEMPLATE*) buffer, parentHandle,
                     DialogProc, (LPARAM) this);
        ::GlobalUnlock(globalHandle);
        return (result != 0);
      }
    }

    return false;   
  }

  Dialog::~Dialog() {
    for (pair<WORD,Control*> entry : idMap) {
      Control* controlPtr = entry.second;
      delete controlPtr;
    }
  }
  
  int Dialog::AddControl(Control* controlPtr) {
    Point topLeft = controlPtr->TopLeft();
    Size controlSize = controlPtr->GetSize();

    if (idMap.empty()) {
      leftMargin = topLeft.X();
      topMargin = topLeft.X();
      maxWidth = topLeft.X() + controlSize.Width();
      maxHeight = topLeft.Y() + controlSize.Height();
    }
    else {
      leftMargin = min(leftMargin, topLeft.X());
      topMargin = min(topMargin, topLeft.Y());
      maxWidth = max(maxWidth, topLeft.X() + controlSize.Width());
      maxHeight = max(maxHeight, topLeft.Y() + controlSize.Height());
    }

    idMap[currentId] = controlPtr;
    return currentId--;
  }

  void Dialog::OnSize(Size newClientSize) {
    pair<double, double> factorPair
      (((double) newClientSize.Width() /
       originalClientSize.Width()),
       ((double) newClientSize.Height() /
       originalClientSize.Height()));

    for (pair<WORD,Control*> entry : idMap) {
      Control* controlPtr = entry.second;
      Rect originalRect = controlPtr->OriginalRect();
      controlPtr->SetWindowDeviceRect(factorPair * originalRect);
    }
  }

  void Dialog::OnReturn() {
    if (TryClose()) {
      ::EndDialog(windowHandle, 1);
    }
  }

  void Dialog::OnEscape() {
    if (TryClose()) {
      ::EndDialog(windowHandle, 0);
    }
  }

  void Dialog::OnClose() {
    if (TryClose()) {
      ::EndDialog(windowHandle, 0);
    }
  }

  INT_PTR CALLBACK DialogProc(HWND dialogHandle, UINT message,
                              WPARAM wordParam, LPARAM longParam){
    switch (message) {
      case WM_INITDIALOG: {
          Dialog* dialogPtr = (Dialog*) longParam;
          dialogPtr->WindowHandle() = dialogHandle;
          dialogMap[dialogHandle] = dialogPtr;
          dialogPtr->originalClientSize =
            dialogPtr->GetClientDeviceSize();
          dialogPtr->OnDialogInit();

          for (pair<WORD,Control*> entry : dialogPtr->IdMap()) {
            WORD controlId = entry.first;
            Control* controlPtr = entry.second;
            controlPtr->WindowHandle() =
              ::GetDlgItem(dialogHandle, controlId);
            controlPtr->OriginalRect() =
              controlPtr->GetWindowDeviceRect();
            controlPtr->OnControlInit(dialogPtr);
          }
        }
        return TRUE;

      case WM_SIZE: {
          Dialog* dialogPtr = dialogMap[dialogHandle];
          assert(dialogPtr != nullptr);
          Size clientSize =
            {LOWORD(longParam), HIWORD(longParam)};
          dialogPtr->OnSize(clientSize);
        }
        return TRUE;

      case WM_CLOSE: {
          Dialog* dialogPtr = dialogMap[dialogHandle];
          assert(dialogPtr != nullptr);
          dialogPtr->OnClose();
        }
        return TRUE;

      case WM_DESTROY: {
          Dialog* dialogPtr = dialogMap[dialogHandle];
          dialogPtr->OnDestroy();
          dialogMap.erase(dialogHandle);
        }
        return TRUE;

      case WM_COMMAND: {
          Dialog* dialogPtr = dialogMap[dialogHandle];
          WORD controlId = LOWORD(wordParam);

          switch (controlId) {
            case IDOK:
              dialogPtr->OnReturn();
              break;

            case IDCANCEL:
              dialogPtr->OnEscape();
              break;

            default: {
                Control* controlPtr =
                  dialogPtr->IdMap()[controlId];
                WORD notificationCode = HIWORD(wordParam);

                switch (notificationCode) {
                  case EN_SETFOCUS:
                    controlPtr->OnGainFocus(dialogPtr);
                    break; 

                  case EN_KILLFOCUS:
                    controlPtr->OnLoseFocus(dialogPtr);
                    break; 

                  case EN_CHANGE:
                    controlPtr->OnChange(dialogPtr);
                    break; 

                  case CBN_SELCHANGE:
                    controlPtr->OnSelect(dialogPtr);
                    break; 

                  case BN_CLICKED:
                    controlPtr->OnClick(dialogPtr);
                    break; 
                }
              }
          }
        }

        return TRUE;
    }

    return FALSE;
  }
};