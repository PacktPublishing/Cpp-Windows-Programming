HDC GetPreparedScrollZoomDC(HWND hWnd, int iZoom);
void PrepareScrollZoomDC(HWND hWnd, HDC hDC, int iZoom);

void DeviceToLogicalZoomSize(HWND hWnd, int iZoom, SIZE* pszSize);
void DeviceToLogicalScrollZoomPoint(HWND hWnd, int iZoom, POINT* pptPoint);
void DeviceToLogicalScrollZoomRect(HWND hWnd, int iZoom, RECT* prcArea);

void LogicalZoomToDeviceSize(HWND hWnd, int iZoom, SIZE* pszSize);
void LogicalScrollZoomToDevicePoint(HWND hWnd, int iZoom, POINT* pptPoint);
void LogicalScrollZoomToDeviceRect(HWND hWnd, int iZoom, RECT* prcArea);

void InvalidateLogicalScrollZoomRect(HWND hWnd, int iZoom, RECT* prcArea, BOOL bRedraw);
