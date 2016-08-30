//       1         2         3         4         5         6
//345678901234567890123456789012345678901234567890123456789012

#include <Windows.h>
#include "UnitScrollZoom.h"

/* ${PrepareScrollZoomDC} prepared the device context for the logical coordinate system. It takes into consideration the scroll bars positions and set up the device coordinate system to match the logical coordinate system. The preparation of the device context does take the scroll bar position into account. We set the coordinate to isotropic state and let a logical unit be one hundredth of a millimeter in both the x and y direction. */
void PrepareScrollZoomDC(HWND hWnd, HDC hDC, int iZoom) {
  double dZoomFactor = iZoom / 100.0;
  int iZoomInverse = (int) (100 / dZoomFactor);
  SetMapMode(hDC, MM_ISOTROPIC);
  SetWindowExtEx(hDC,iZoomInverse*GetDeviceCaps(hDC,HORZSIZE),
               iZoomInverse*GetDeviceCaps(hDC,VERTSIZE),NULL);
  SetViewportExtEx(hDC, GetDeviceCaps(hDC, HORZRES),
                   GetDeviceCaps(hDC, VERTRES), NULL);

  { int xScrollPos = GetScrollPos(hWnd, SB_HORZ),
     yScrollPos = GetScrollPos(hWnd, SB_VERT);   
    SetWindowOrgEx(hDC, xScrollPos, yScrollPos, NULL);
  }
}
/* ${GetPreparedScrollZoomDC} simplifies the code since the device context is being prepared when initialized. */
HDC GetPreparedScrollZoomDC(HWND hWnd, int iZoom) {
  HDC hDC = GetDC(hWnd);
  PrepareScrollZoomDC(hWnd, hDC, iZoom);
  return hDC;
}
void LogicalZoomToDeviceSize(HWND hWnd, int iZoom,
                             SIZE* pszSize) {
  RECT rcRect = {0, 0, pszSize->cx, pszSize->cy};
  LogicalScrollZoomToDeviceRect(hWnd, iZoom, &rcRect);
  pszSize->cx = rcRect.right - rcRect.left;
  pszSize->cy = rcRect.bottom - rcRect.top;
}
/* ${LogicalScrollZoomToDeviceRect} transforms the given area from logical units (hundredth of millimeters) to device units (pixels), which are needed when invalidating a part of the client area and updating the caret. */
void LogicalScrollZoomToDevicePoint(HWND hWnd, int iZoom,
                                    POINT* pptPoint) {
  HDC hDC = GetPreparedScrollZoomDC(hWnd, iZoom);
  LPtoDP(hDC, pptPoint, 1);
  ReleaseDC(hWnd, hDC);
}

void DeviceToLogicalZoomSize(HWND hWnd,int iZoom,SIZE*pszSize) {
  RECT rcRect = {0, 0, pszSize->cx, pszSize->cy};
  DeviceToLogicalScrollZoomRect(hWnd, iZoom, &rcRect);
  pszSize->cx = rcRect.right - rcRect.left;
  pszSize->cy = rcRect.bottom - rcRect.top;
}

/* ${DeviceToLogicalScrollZoomPoint} transforms a point from device units into logical units. This function is only called when the user resize the windoe in order to set the scroll page value and when the user presses, moves, releases, and double clicks the mouse. The size of the client area and the mouse corrdinates are given in device units that need to be translated into logical units. */
void DeviceToLogicalScrollZoomPoint(HWND hWnd, int iZoom,
                                    POINT* pptPoint) {
  HDC hDC = GetPreparedScrollZoomDC(hWnd, iZoom);
  DPtoLP(hDC, pptPoint, 1);
  ReleaseDC(hWnd, hDC);
}
/* ${LogicalScrollZoomToDeviceRect} transforms the given area from logical units (hundredth of millimeters) to device units (pixels), which are needed when invalidating a part of the client area and updating the caret. */
void LogicalScrollZoomToDeviceRect(HWND hWnd, int iZoom,
                                   RECT* prcArea) {
  POINT ptTopLeft = {prcArea->left, prcArea->top},
        ptBottomRight = {prcArea->right, prcArea->bottom};
  LogicalScrollZoomToDevicePoint(hWnd, iZoom, &ptTopLeft);
  LogicalScrollZoomToDevicePoint(hWnd, iZoom, &ptBottomRight);

  prcArea->left = ptTopLeft.x;
  prcArea->top = ptTopLeft.y;
  prcArea->right = ptBottomRight.x;
  prcArea->bottom = ptBottomRight.y;
}
/* ${DeviceToLogicalScrollZoomRect} is called when logical units need to be transfered into device units, which only happens on two occasions: when a part of the client area needs to be repainted (invalidated) and when the caret marker needs to be updated. */
void DeviceToLogicalScrollZoomRect(HWND hWnd, int iZoom,
                                   RECT* prcArea) {
  POINT ptTopLeft = {prcArea->left, prcArea->top},
        ptBottomRight = {prcArea->right, prcArea->bottom};
  DeviceToLogicalScrollZoomPoint(hWnd, iZoom, &ptTopLeft);
  DeviceToLogicalScrollZoomPoint(hWnd, iZoom, &ptBottomRight);

  prcArea->left = ptTopLeft.x;
  prcArea->top = ptTopLeft.y;
  prcArea->right = ptBottomRight.x;
  prcArea->bottom = ptBottomRight.y;
}

void InvalidateLogicalScrollZoomRect(HWND hWnd, int iZoom,
                                    RECT*prcArea,BOOL bRedraw) {
  LogicalScrollZoomToDeviceRect(hWnd, iZoom, prcArea);
  InvalidateRect(hWnd, prcArea, bRedraw);
}