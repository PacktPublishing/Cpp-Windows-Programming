#include <Windows.h>
#include <Assert.h>
#include <String.h>
#include <TChar.h>

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message,
                            WPARAM wordParam, LPARAM longParam);

int WINAPI WinMain(HINSTANCE instanceHandle,
                   HINSTANCE prevInstanceHandle,
                   char* commandLine, int commandShow) {
  WNDCLASS windowClass;
  memset(&windowClass, 0, sizeof windowClass);
  windowClass.hInstance = instanceHandle;
  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  windowClass.lpfnWndProc = WindowProc;
  windowClass.lpszClassName = TEXT("window");
  RegisterClass(&windowClass);

  HWND windowHandle = CreateWindowEx(0, TEXT("window"), NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, CreateMenu(), instanceHandle, NULL);
  assert(windowHandle != NULL);
  ShowWindow(windowHandle, commandShow);
  RegisterTouchWindow(windowHandle, 0);
  SetWindowText(windowHandle, TEXT("Hello Window"));

  MSG message;
  while (GetMessage(&message, NULL, 0, 0)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }

  return ((int) message.wParam);
}

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message,
                            WPARAM wordParam, LPARAM longParam){
  switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT paintStruct;
        HDC deviceContextHandle = BeginPaint(windowHandle, &paintStruct);
        SetMapMode(deviceContextHandle, MM_ISOTROPIC);

        int horizontalSize = 100 * GetDeviceCaps(deviceContextHandle, HORZSIZE),
              verticalSize = 100 * GetDeviceCaps(deviceContextHandle,VERTSIZE);
        SetWindowExtEx(deviceContextHandle, horizontalSize, verticalSize, NULL);

        int horizontalResolution = (int) GetDeviceCaps(deviceContextHandle,HORZRES),
            verticalResolution = (int) GetDeviceCaps(deviceContextHandle,VERTRES);
        SetViewportExtEx(deviceContextHandle, horizontalResolution, verticalResolution, NULL);
        
        int horizontalScroll = GetScrollPos(windowHandle, SB_HORZ),
            verticalScroll = GetScrollPos(windowHandle, SB_VERT);
        SetWindowOrgEx(deviceContextHandle, horizontalScroll, verticalScroll, NULL);
        
        RECT clientRect;
        GetClientRect(windowHandle, &clientRect);
        POINT bottomRight = {clientRect.right, clientRect.bottom};
        DPtoLP(deviceContextHandle, &bottomRight, 1);
        clientRect.right = bottomRight.x;
        clientRect.top = bottomRight.y;

        LOGFONT logFont;
        memset(&logFont, 0, sizeof logFont);
        _tcscpy_s(logFont.lfFaceName, LF_FACESIZE, TEXT("Times New Roman"));
        int fontSize = 12;
        logFont.lfHeight = (int) ((2540.0 * fontSize) / 72);
        logFont.lfWeight = FW_BOLD;
        logFont.lfItalic = FALSE;

        HFONT fontHandle = CreateFontIndirect(&logFont);
        HFONT oldFontHandle = (HFONT) SelectObject(deviceContextHandle, fontHandle);

        COLORREF black = RGB(0, 0, 0), white = RGB(255, 255, 255);
        SetTextColor(deviceContextHandle, black);
        SetBkColor(deviceContextHandle, white);

        TCHAR* textPtr = TEXT("Hello, Small Windows!");
        DrawText(deviceContextHandle, textPtr, _tcslen(textPtr),
                 &clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        SelectObject(deviceContextHandle, oldFontHandle);
        DeleteObject(fontHandle);
        EndPaint(windowHandle, &paintStruct);
      }
      return 0;

    case WM_CLOSE:
      PostQuitMessage(0);
  }

  return DefWindowProc(windowHandle, message, wordParam, longParam);
}