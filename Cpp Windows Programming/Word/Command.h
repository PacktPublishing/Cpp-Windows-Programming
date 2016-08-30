void OnPrepareCommand(HWND WPARAM wParam, LPARAM lParam);
void OnCommand(HWND WPARAM wParam, LPARAM lParam);

bool TryClose();
void UpdateWindowHead();

void OnFileNew();
void OnFileOpen();
void FileOpen(HWND TCHAR stPath[]);
bool OnFileSave();
bool OnFileSaveAs();
bool FileRead(HWND TCHAR stFileName[]);
bool FileWrite(HWND TCHAR stFileName[]);
void OnAppExit();

void OnEditCut();
void OnEditCopy();
void OnEditPaste();
void OnEditPageBreak();

bool IsAlignment(int iAlignment);
void SetAlignment(HWND int iAlignment);
void OnFormatFont();
void OnFormatIncZoom();
void OnFormatDecZoom();
void TouchZoom(HWND int iInitZoom, POINT ptOldFirst, POINT ptOldSecond, POINT ptNewFirst, POINT ptNewSecond);
void OnAppAbout();
