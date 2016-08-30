void OnFilePrint();
bool PrintPage(HWND HDC hDC, TEMPLATE_INFO* pTempInfo,
               int iPage, int iCopy);
bool CALLBACK DialogProc(HWND hDlg, UINT uMessage, WPARAM wParam,
                         LPARAM lParam);
bool CALLBACK AbortProc(HDC hDC, int iCode);
void Print(HWND HDC hDC, int iPage, bool bPreview, TEMPLATE_INFO* pSetupInfo);
