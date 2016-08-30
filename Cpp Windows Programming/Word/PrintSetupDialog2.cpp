/*
#include <Windows.h>
#include <StdIO.h>
#include <TChar.h>
#include <WChar.h>
#include <String.h>
#include <Assert.h>
#include "Resource.h"
#include "..\\List.h"
#include "..\\Set.h"
#include "..\\Unit.h"
#include "..\\UnitScroll.h"
#include "UnitScrollZoom.h"
#include "Position.h"
#include "Paragraph.h"
#include "Calculate.h"
#include "Resource.h"
#include "Word.h"
#include "Command.h"
#include "Keyboard.h"
#include "Template.h"
#include "PrintSetupDialog.h"
#include "PrintPreview.h"
#include "Print.h"
*/

#include "..\\SmallWindows\\SmallWindows.h"
#include "PrintSetupDialog.h"

PRINT_INFO g_printInfo = { DEFAULT_MARGIN, DEFAULT_MARGIN,
                          DEFAULT_MARGIN, DEFAULT_MARGIN,
                          TEXT(""), true, {0}, 0,
                          TEXT(""), true, {0}, 0, true};

void OnFilePrintSetup() {
  int iPageWidth = m_iPageWidth, iPageHeight = m_iPageHeight;

  if (DialogBox(g_hInstance, MAKEINTRESOURCE(IDR_SETUPDIALOG),
                &SetupDialogProc) &&
      ((iPageWidth != m_iPageWidth) ||
       (iPageHeight != m_iPageHeight))) {
    int iIndex;
    for (iIndex = 0; iIndex < ListSize(&m_paraList);
         ++iIndex) {
      Paragraph* pParagraph;
      ListGet(&m_paraList, iIndex, &pParagraph);
      ParagraphCalculate(pParagraph, NULL, 0, NULL);
    }

    UpdateParagraphAndPageList();
    UpdateCaret();
    Invalidate();
    UpdateWindow();
  }
}

LOGFONT g_lfHeaderFont, g_lfFooterFont;
COLORREF g_crHeaderColor, g_crFooterColor;

bool CALLBACK SetupDialogProc(HWND hDlg, UINT uiMessage,
                              WPARAM wParam, LPARAM lParam) {
  switch (uiMessage) {
    case WM_INITDIALOG:
      SetDlgItemInt(hDlg, ID_LEFT_MARGIN,
                    g_printInfo.m_xLeftMargin, true);
      SetDlgItemInt(hDlg, ID_TOP_MARGIN,
                    g_printInfo.m_yTopMargin, true);
      SetDlgItemInt(hDlg, ID_RIGHT_MARGIN,
                    g_printInfo.m_xRightMargin, true);
      SetDlgItemInt(hDlg, ID_BOTTOM_MARGIN,
                    g_printInfo.m_yBottomMargin, true);

      SetDlgItemText(hDlg, ID_HEADER_TEXT,
                     g_printInfo.m_stHeaderText);
      CheckDlgButton(hDlg, ID_HEADER_FIRSTPAGE,
                     g_printInfo.m_bHeaderFirstPage);
      g_lfHeaderFont = g_printInfo.m_lfHeaderFont;
      g_crHeaderColor = g_printInfo.m_crHeaderColor;

      SetDlgItemText(hDlg, ID_FOOTER_TEXT,
                     g_printInfo.m_stFooterText);
      CheckDlgButton(hDlg, ID_FOOTER_FIRSTPAGE,
                     g_printInfo.m_bFooterFirstPage);
      g_lfFooterFont = g_printInfo.m_lfFooterFont;
      g_crFooterColor = g_printInfo.m_crFooterColor;

      { HWND hLeftMargin = GetDlgItem(hDlg, ID_LEFT_MARGIN);
        SetFocus(hLeftMargin);
      }

      CheckDlgButton(hDlg, ID_PRINT_FRAME,
                     g_printInfo.m_bPrintFrame);
      return true;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case IDOK:
          if (SaveDialog(hDlg)) {
            EndDialog(hDlg, true);
          }
          break;

        case IDCANCEL:
          EndDialog(hDlg, false);
          break;

        case ID_HEADER_FONT:
          FontDialog(hDlg, &g_lfHeaderFont, &g_crHeaderColor);
          break;

        case ID_FOOTER_FONT:
          FontDialog(hDlg, &g_lfFooterFont, &g_crFooterColor);
          break;
      }
      return true;

    default:
      return false;
  }
}

bool SaveDialog(HWND hDlg) {
  bool bOk;
  PRINT_INFO newPrintInfo;

  newPrintInfo.m_xLeftMargin =
               GetDlgItemInt(hDlg, ID_LEFT_MARGIN, &bOk,true);
  if (!bOk) {
    MessageBox(hDlg, TEXT("Left Margin is not an Integer."),
               TEXT("Left Margin"), MB_OKCANCEL);
    return false;
  }

  newPrintInfo.m_yTopMargin =
               GetDlgItemInt(hDlg, ID_TOP_MARGIN, &bOk, true);
  if (!bOk) {
    MessageBox(hDlg, TEXT("Top Margin is not an Integer."),
               TEXT("Top Margin"), MB_OK);
    return false;
  }

  newPrintInfo.m_xRightMargin =
               GetDlgItemInt(hDlg, ID_RIGHT_MARGIN,&bOk,true);
  if (!bOk) {
    MessageBox(hDlg, TEXT("Right Margin is not an Integer."),
               TEXT("Right Margin"), MB_OK);
    return false;
  }

  newPrintInfo.m_yBottomMargin =
               GetDlgItemInt(hDlg,ID_BOTTOM_MARGIN,&bOk,true);
  if (!bOk) {
    MessageBox(hDlg, TEXT("Bottom Margin is not an Integer."),
               TEXT("Bottom Margin"), MB_OK);
    return false;
  }

  { int iPageWidth = PAGE_TOTALWIDTH -
                     (newPrintInfo.m_xLeftMargin +
                      newPrintInfo.m_xRightMargin),
        iPageHeight = PAGE_TOTALHEIGHT -
                      (newPrintInfo.m_yTopMargin +
                       newPrintInfo.m_yBottomMargin);

    if ((iPageWidth <= 0) ||(iPageHeight <= 0)) {
      MessageBox(hDlg, TEXT("The page width and height must ")
                 TEXT("be positive."), TEXT("Margins"),MB_OK);
      return false;
    }

    m_iPageWidth = iPageWidth;
    m_iPageHeight = iPageHeight;
  }

  GetDlgItemText(hDlg, ID_HEADER_TEXT,
                 newPrintInfo.m_stHeaderText, STRING_SIZE);
  newPrintInfo.m_bHeaderFirstPage =
               IsDlgButtonChecked(hDlg, ID_HEADER_FIRSTPAGE);
  newPrintInfo.m_lfHeaderFont = g_lfHeaderFont;
  newPrintInfo.m_crHeaderColor = g_crHeaderColor;

  GetDlgItemText(hDlg, ID_FOOTER_TEXT,
                 newPrintInfo.m_stFooterText, STRING_SIZE);
  newPrintInfo.m_bFooterFirstPage =
               IsDlgButtonChecked(hDlg, ID_FOOTER_FIRSTPAGE);
  newPrintInfo.m_lfHeaderFont = g_lfHeaderFont;
  newPrintInfo.m_crHeaderColor = g_crHeaderColor;

  newPrintInfo.m_bPrintFrame =
               IsDlgButtonChecked(hDlg, ID_PRINT_FRAME);

  if (memcmp(&g_printInfo, &newPrintInfo,
             sizeof(PRINT_INFO)) != 0) {
    g_bDirty = true;
  }

  g_printInfo = newPrintInfo;
  return true;
}

bool FontDialog(HWND hDlg,LOGFONT* plfFont,COLORREF* pcrColor) {
  CHOOSEFONT chooseFont;
  chooseFont.lStructSize    = sizeof chooseFont;
  chooseFont.hwndOwner      = hDlg;
  chooseFont.hDC            = NULL;
  chooseFont.iPointSize     = 0;
  chooseFont.Flags          =
    CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS|CF_EFFECTS;
  chooseFont.lCustData      = 0;
  chooseFont.lpfnHook       = NULL;
  chooseFont.lpTemplateName = NULL;
  chooseFont.hInstance      = NULL;
  chooseFont.lpszStyle      = NULL;
  chooseFont.nFontType      = 0;
  chooseFont.nSizeMin       = 0;
  chooseFont.nSizeMax       = 0;

  chooseFont.lpLogFont = plfFont;
  chooseFont.rgbColors = *pcrColor;

  if (ChooseFont(&chooseFont)) {
    *plfFont = *chooseFont.lpLogFont;
    *pcrColor = chooseFont.rgbColors;
    return true;
  }

  return false;
}