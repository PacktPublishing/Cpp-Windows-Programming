#if 0

class WordWindow : public Window {
  public:
    /* There are two modes for the application: edit state with the caret visible and mark state with one or several characters marked. */ 
    const enum {WS_EDIT, WS_MARK};

    #define STRING_SIZE 256

    #define PAGE_BREAK TEXT("-- Page Break --")

    /* Each page holds ist first and last paragraph as well as its height. */
    typedef struct  {
      int m_iFirstParagraph, m_iLastParagraph, m_iHeight;
    } PAGE_INFO;

    const enum {KM_INSERT, KM_OVERWRITE};

    extern UINT g_uiInternalFormat;

    extern int g_iWordStatus, g_iKeyboardState, g_iZoom;

    extern POSITION g_psEdit, g_psFirstMark, g_psLastMark;
    extern bool g_bDirty;

    extern FONT_INFO* g_pNextFont;
    extern TCHAR g_stPath[MAX_PATH];

    extern const TCHAR APP_NAME[];
    extern HWND g_hDlgPrint;
    extern bool g_bUserAbort;
    extern HINSTANCE g_hInstance;

    extern LIST g_paragraphList, g_pageList;

    int WINAPI WinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       PSTR stCmdLine, int iCmdShow);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMessage,
                             WPARAM wParam, LPARAM lParam);
    void UpdateCaret(HWND hWnd);
    void RepaintRectangleSet(HWND hWnd, SET* pRepaintSet);
    //void InvalidateLogicalScrollZoomRect(HWND hWnd, RECT* prcArea);
    void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnHorizontalScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnVerticalScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnMouseDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnMouseUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnDoubleClick(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnTouch(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void UpdateParagraphAndPageList(HWND hWnd);
    SIZE GetPageBreakSize(HWND hWnd);
    void DeleteText(HWND hWnd, SET* pRepaintSet);
    void Draw(HDC hDC);
    bool CALLBACK AbortProc(HDC hDC, int iCode);
    bool CALLBACK DialogProc(HWND hDlg, UINT uMessage,
                             WPARAM wPAram, LPARAM lParam);

    void OnPrepareCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);

    bool TryClose(HWND hWnd);
    void UpdateWindowHead(HWND hWnd);

    void OnFileNew(HWND hWnd);
    void OnFileOpen(HWND hWnd);
    void FileOpen(HWND hWnd, TCHAR stPath[]);
    bool OnFileSave(HWND hWnd);
    bool OnFileSaveAs(HWND hWnd);
    bool FileRead(HWND hWnd, TCHAR stFileName[]);
    bool FileWrite(HWND hWnd, TCHAR stFileName[]);
    void OnAppExit(HWND hWnd);

    void OnEditCut(HWND hWnd);
    void OnEditCopy(HWND hWnd);
    void OnEditPaste(HWND hWnd);
    void OnEditPageBreak(HWND hWnd);

    bool IsAlignment(int iAlignment);
    void SetAlignment(HWND hWnd, int iAlignment);
    void OnFormatFont(HWND hWnd);
    void OnFormatIncZoom(HWND hWnd);
    void OnFormatDecZoom(HWND hWnd);
    void TouchZoom(HWND hWnd, int iInitZoom, POINT ptOldFirst, POINT ptOldSecond, POINT ptNewFirst, POINT ptNewSecond);
    void OnAppAbout(HWND hWnd);
};
#endif