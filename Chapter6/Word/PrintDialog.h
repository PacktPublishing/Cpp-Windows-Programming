#ifndef _PRINT_DIALOG_
#define _PRINT_DIALOG_

namespace SmallWindows {
  class Window;

  class PrintDialog {
    public:
      PrintDialog(Window* pWindow);
      bool Display();

      void OnFilePrint();
      bool PrintPage(HWND HDC hDC, TEMPLATE_INFO* pTempInfo, int iPage, int iCopy);

      friend bool CALLBACK DialogProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam);
      friend bool CALLBACK AbortProc(HDC hDC, int iCode);

      void Print(HWND HDC hDC, int iPage, bool bPreview, TEMPLATE_INFO* pSetupInfo);

    private:
  };
};
#endif
