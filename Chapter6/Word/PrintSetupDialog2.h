typedef struct {
  int m_xLeftMargin, m_yTopMargin,
      m_xRightMargin, m_yBottomMargin;

  TCHAR m_stHeaderText[STRING_SIZE];
  bool m_bHeaderFirstPage;
  LOGFONT m_lfHeaderFont;
  COLORREF m_crHeaderColor;

  TCHAR m_stFooterText[STRING_SIZE];
  bool m_bFooterFirstPage;
  LOGFONT m_lfFooterFont;
  COLORREF m_crFooterColor;

  bool m_bPrintFrame;
} PRINT_INFO;

extern PRINT_INFO g_printInfo;

void OnFilePrintSetup();
bool CALLBACK SetupDialogProc(HWND UINT uiMessage,
                              WPARAM wParam, LPARAM lParam);
bool SaveDialog(HWND hDlg);
bool FontDialog(HWND hDlg, LOGFONT* plfFont, COLORREF* pcrColor);