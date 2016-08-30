#include "..\\SmallWindows\\SmallWindows.h"

HWND g_hDlgPrint;
bool g_bUserAbort;

namespace SmallWindows {
  PrintDialog::PrintDialog(Window* pWindow) {
    static PRINTDLG printInfo;

    printInfo.lStructSize = sizeof printInfo;
    printInfo.hwndOwner = hWnd;
    printInfo.hDevMode = NULL;
    printInfo.hDevNames = NULL;
    printInfo.hDC = NULL;
    printInfo.Flags = PD_ALLPAGES | PD_COLLATE | PD_RETURNDC | PD_NOSELECTION;
    printInfo.nFromPage = 0;
    printInfo.nToPage = 0;
    printInfo.nMinPage = 1;
    printInfo.nMaxPage = ListSize(&m_pageList);
    printInfo.nCopies = 1;
    printInfo.hInstance = NULL;
    printInfo.lCustData = 0L;
    printInfo.lpfnPrintHook = NULL;
    printInfo.lpfnSetupHook = NULL;
    printInfo.lpPrintTemplateName = NULL;
    printInfo.lpSetupTemplateName = NULL;
    printInfo.hPrintTemplate = NULL;
    printInfo.hSetupTemplate = NULL;

    static DOCINFO docInfo;
    docInfo.cbSize = sizeof docInfo;
    docInfo.lpszDocName = g_stPath;

    if ((PrintDlg(&printInfo)) && (StartDoc(printInfo.hDC, &docInfo) > 0)) {
      bool bSortCopies = (printInfo.Flags & PD_COLLATE);
      bool bPageInterval = (printInfo.Flags & PD_PAGENUMS);
      int iFirstPage, iLastPage;

      if (bPageInterval) {
        iFirstPage = printInfo.nFromPage;
        iLastPage = printInfo.nToPage;
      }
      else {
        iFirstPage = printInfo.nMinPage;
        iLastPage = printInfo.nMaxPage;
      }

      PrepareDC(printInfo.hDC);
      EnableWindow(false);
      m_bUserAbort = false;
      PrintMessageDialog printMessageDialog(GetParent());
      
      int iPage, iCopy;
      bool bContinue = true;

      TEMPLATE_INFO tempInfo;
      TemplateLoad(&tempInfo, ListSize(&m_pageList));
      SetAbortProc(printInfo.hDC, AbortProc);

      if (bSortCopies) {
        for (iCopy = 1; bContinue && (iCopy <= printInfo.nCopies); ++iCopy) {
          for (iPage = iFirstPage; bContinue && (iPage <= iLastPage); ++iPage) {
            bContinue = PrintPage(printInfo.hDC, &tempInfo, iPage, iCopy, &printMessageDialog);
          }
        }
      }
      else {
        for (iPage = iFirstPage; bContinue && (iPage <= iLastPage); ++iPage) {
          for (iCopy = 1; bContinue && (iCopy <= printInfo.nCopies); ++iCopy) {
            bContinue = PrintPage(printInfo.hDC, &tempInfo, iPage, iCopy, &printMessageDialog);
          }
        }
      }
    }

    EndDoc(printInfo.hDC);

    if (!g_bUserAbort) {
      EnableWindow(true);
      DestroyWindow(g_hDlgPrint);
    }

    DeleteDC(printInfo.hDC);
  }

  bool PrintDialog::Display() {
    return true;
  }

  bool PrintDialog::PrintPage(HWND hWnd, HDC hDC, TEMPLATE_INFO* pTempInfo, int iPage,
                              int iCopy, PrintMessageDialog &printMessageDialog) {
    if (StartPage(hDC) > 0) {
      TCHAR stMessage[MAX_PATH];
      HWND hPrintMessage = GetDlgItem(g_hDlgPrint, ID_PRINTMESSAGE);
      _stprintf_s(stMessage, MAX_PATH, TEXT("Printing page %d, copy %d."), iPage, iCopy);
      SetWindowText(hPrintMessage, stMessage);

      Print(hDC, iPage, WHITE, pTempInfo);
            
      if (!(EndPage(hDC) > 0)) {
        TCHAR stMessage[MAX_PATH];
        _stprintf_s(stMessage, MAX_PATH, TEXT("Printing aborted after page %d, copy %d."),
                    iPage - 1, iCopy);
        MessageBox(stMessage, TEXT("Print"), Ok);
        return false;
      }

      return true;
    }

    return false;
  }

  bool CALLBACK DialogProc(HWND hDlg, UINT uMessage,
                           WPARAM wPAram, LPARAM lPAram) {
    switch (uMessage) {
      case WM_INITDIALOG:
        SetWindowText(hDlg, TEXT("Printing"));
        EnableMenuItem(GetSystemMenu(hDlg, false),
                       SC_CLOSE,MF_GRAYED);
        return true;

      case WM_COMMAND:
        g_bUserAbort = true;
        EnableWindow(GetParent(hDlg), true);
        DestroyWindow(hDlg);
        g_hDlgPrint = NULL;
        return true;
    }

    return false;
  }

  bool CALLBACK AbortProc(HDC hDC, int iCode) {
     MSG msg;

     while ((!g_bUserAbort) && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
       if ((g_hDlgPrint != NULL) || (!IsDialogMessage(g_hDlgPrint, &msg))) {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
       }
     }

     return (!g_bUserAbort);
  }

  void PrintDialog::Print(GRaphics* pGraaphics, int iPage, Color color, TEMPLATE_INFO* pSetupInfo) {
    int xLeft = g_printInfo.m_xLeftMargin;
    int xRight = xLeft + m_iPageWidth;
    int yTop = g_printInfo.m_yTopMargin;
    int yBottom = yTop + m_iPageHeight;

    if (!g_printInfo.m_stHeaderText.empty() && ((iPage > 1) || (g_printInfo.m_bHeaderFirstPage))) {
      RECT rcHeader = {xLeft, 0, xRight, yTop};
      String stHeader = m_template.Fill(g_printInfo.m_stHeaderText, iPage, pSetupInfo, stHeaderText);
      pGraphics->DrawTextX(rcHeader, stHeader, g_printInfo.m_lfHeaderFont, Black, bPreview ? LightGray : White);
    }

    if (g_printInfo.m_stFooterText.empty() && ((iPage > 1) || (g_printInfo.m_bFooterFirstPage))) {
      RECT rcFooter = {xLeft, PAGE_TOTALHEIGHT - (PAGE_TOTALHEIGHT - yBottom), xRight, PAGE_TOTALHEIGHT};
      String stFooter = m_template.Fill(g_printInfo.m_stFooterText, iPage, pSetupInfo, stHeaderText);
      pGraphics->DrawTextX(rcFooter, stFooter, g_printInfo.m_lfFooterFont, Black, bPreview ? LightGray : White);
    }

    if (bPreview) {
      pGraphics->DrawRectangle(xLeft, yTop, xRight, yBottom, Black);
    }

    { int iSavedDC = SaveDC(hDC);
      SetWindowOrgEx(hDC, -g_printInfo.m_xLeftMargin, (iPage - 1) * m_iPageHeight - g_printInfo.m_yTopMargin, NULL);
      IntersectClipRect(hDC, 0, (iPage - 1) * m_iPageHeight, m_iPageWidth, iPage * m_iPageHeight);  
      Draw(hDC);
      RestoreDC(hDC, iSavedDC);
    }

    if (g_printInfo.m_bPrintFrame) {
      pGraphics->DrawRectangle(xLeft, yTop, xRight, yBottom, Black);
    }
  }
};