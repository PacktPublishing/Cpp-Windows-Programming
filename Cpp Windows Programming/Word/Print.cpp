#include "..\\SmallWindows\\SmallWindows.h"
#include "Template.h"
#include "PrintSetupDialog.h"
//#include "PrintMessageDialog.h"
#include "PrintPreviewDocument.h"
#include "Position.h"
#include "CharInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

HWND g_hDlgPrint;
bool g_bUserAbort;

void WordDocument::OnStandardPrint() {
  int iPageNum = m_pageList.size();
  PrintDialog printDialog(this, iPageNum);

  if (printDialog.Display()) {
    bool bSorted = printDialog.Sorted();
    int iCopies = printDialog.Copies(),
        iFirstPage = printDialog.FirstPage(),
        iLastPage = printDialog.LastPage();
    Graphics* pPrintGraphics = printDialog.GetGraphics();

    if (StartDocument(pPrintGraphics, GetName())) {
      PrepareGraphics(pPrintGraphics);
      EnableWindow(m_hWnd, false);
      g_bUserAbort = false;
       
//      PrintMessageDialog* pDialog = new PrintMessageDialog(this);
      m_template.Load(this, iPageNum);
//      SetAbortProcedure(pPrintGraphics, AbortProc);

      if (bSorted) {
        for (int iCopy = 1; iCopy <= iCopies; ++iCopy) {
          for (int iPage = iFirstPage; iPage <= iLastPage; ++iPage) {
            PrintPage(pPrintGraphics, iPage);
          }
        }
      }
      else {
        for (int iPage = iFirstPage; iPage <= iLastPage; ++iPage) {
          for (int iCopy = 1; iCopy <= iCopies; ++iCopy) {
            PrintPage(pPrintGraphics, iPage);
          }
        }
      }

/*
      if (bSorted) {
        bool bContinue = true;
        for (int iCopy = 1; bContinue && (iCopy <= iCopies); ++iCopy) {
          for (int iPage = iFirstPage; bContinue && (iPage <= iLastPage); ++iPage) {
//            DisplayPrintMessage(pDialog, iPage, iCopy);
            bContinue = PrintPage(pPrintGraphics, iPage, iCopy);
          }
        }
      }
      else {
        bool bContinue = true;
        for (int iPage = iFirstPage; bContinue && (iPage <= iLastPage); ++iPage) {
          for (int iCopy = 1; bContinue && (iCopy <= iCopies); ++iCopy) {
//            DisplayPrintMessage(pDialog, iPage, iCopy);
            bContinue = PrintPage(pPrintGraphics, iPage, iCopy);
          }
        }
      }
*/
      EndDocument(pPrintGraphics);

      if (!g_bUserAbort) {
        EnableWindow(m_hWnd, true);
        ::DestroyWindow(g_hDlgPrint);
      }

      delete pPrintGraphics;
//      delete pDialog;
    }
  }
}

/*
void WordDocument::DisplayPrintMessage(PrintMessageDialog* pDialog, int iPage, int iCopy) {
  TCHAR szMessage[MAX_PATH];
  _stprintf_s(szMessage, MAX_PATH, TEXT("Printing page %d, copy %d."), iPage, iCopy);
  pDialog->SetMessage(szMessage);
}
*/

void WordDocument::PrintPage(Graphics* pPrintGraphics, int iPage) {
  if (StartPage(pPrintGraphics)) {
    OnPrint(pPrintGraphics, iPage);        
    EndPage(pPrintGraphics);
  }
}

/*
bool WordDocument::PrintPage(Graphics* pPrintGraphics, int iPage, int iCopy) {
  if (StartPage(pPrintGraphics)) {
    Print(pPrintGraphics, false, iPage);

    if (!EndPage(pPrintGraphics)) {
      TCHAR szMessage[MAX_PATH];
      _stprintf_s(szMessage, MAX_PATH, TEXT("Printing aborted after page %d, copy %d."), iPage - 1, iCopy);
      MessageBox(szMessage, TEXT("Print"), Ok);
      return false;
    }

    return true;
  }

  return false;
}
*/
/*
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMessage,
                         WPARAM wPAram, LPARAM lPAram) {
  switch (uMessage) {
    case WM_INITDIALOG:
      SetWindowText(hDlg, TEXT("Printing"));
      EnableMenuItem(GetSystemMenu(hDlg, false), SC_CLOSE, MF_GRAYED);
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
*/

/*
BOOL CALLBACK AbortProc(HDC hDC, int iCode) {
   MSG msg;

   while ((!g_bUserAbort) && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
     if ((g_hDlgPrint != NULL) || (!IsDialogMessage(g_hDlgPrint, &msg))) {
       TranslateMessage(&msg);
       DispatchMessage(&msg);
     }
   }

   return (!g_bUserAbort);
}
*/

void WordDocument::OnPrint(Graphics* pGraphics, int iPage) {
  int xLeft = m_printSetupInfo.m_xLeftMargin,
      xRight = PAGE_TOTALWIDTH - m_printSetupInfo.m_xLeftMargin,
      yTop = m_printSetupInfo.m_yTopMargin,
      yBottom = PAGE_TOTALHEIGHT - m_printSetupInfo.m_yTopMargin;

  int iPageWidth = xRight - xLeft, iPageHeight = yBottom - yTop;

  if (!m_printSetupInfo.m_stHeader.empty() && ((iPage > 1) || (m_printSetupInfo.m_bHeaderFirst))) {
    Rect rcHeader(xLeft, 0, xRight, yTop);
    String stHeader = m_template.Fill(m_printSetupInfo.m_stHeader, iPage);
    pGraphics->DrawTextArea(rcHeader, stHeader, m_printSetupInfo.m_headerFont, Black, White);
  }

  if (!m_printSetupInfo.m_stFooter.empty() && ((iPage > 1) || (m_printSetupInfo.m_bFooterFirst))) {
    Rect rcFooter(xLeft, yBottom, xRight, PAGE_TOTALHEIGHT);
    String stFooter = m_template.Fill(m_printSetupInfo.m_stFooter, iPage);
    pGraphics->DrawTextArea(rcFooter, stFooter, m_printSetupInfo.m_footerFont, Black, White);
  }

/*
  if (bPreview) {
    pGraphics->FillRectangle(xLeft, yTop, xRight, yBottom, White, White);
  }
*/

  int iSave = pGraphics->Save();
  Point ptOrigion(-m_printSetupInfo.m_xLeftMargin, (iPage - 1) * iPageHeight - m_printSetupInfo.m_yTopMargin);
  pGraphics->SetOrigion(ptOrigion);

  Rect rcClip(0, (iPage - 1) * iPageHeight, iPageWidth, iPage * iPageHeight);
  pGraphics->IntersectClip(rcClip);

  OnDraw(pGraphics);
  pGraphics->Restore(iSave);

  if (m_printSetupInfo.m_bFrame) {
    pGraphics->DrawRectangle(xLeft, yTop, xRight, yBottom, Black);
  }
}