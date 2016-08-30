#include "SmallWindows.h"
namespace SmallWindows {
  Graphics::Graphics(Window* windowPtr, HDC deviceContextHandle)
   :windowPtr(windowPtr),
    deviceContextHandle(deviceContextHandle) {
    // Empty.
  }

  int Graphics::Save() {
    return ::SaveDC(deviceContextHandle);
  }

  void Graphics::Restore(int saveId) {
    ::RestoreDC(deviceContextHandle, saveId);
  }

  void Graphics::SetOrigin(Point centerPoint) {
    ::SetWindowOrgEx(deviceContextHandle, centerPoint.X(),
                     centerPoint.Y(), nullptr);
  }

  void Graphics::IntersectClip(Rect clipRect) {
    ::IntersectClipRect(deviceContextHandle, clipRect.Left(),
               clipRect.Top(),clipRect.Right(),clipRect.Bottom());
  }

  void Graphics::DrawLine(Point startPoint, Point endPoint,
                     Color color, PenStyle penStyle/* = Solid */){
    HPEN penHandle = ::CreatePen(penStyle, 0, color.ColorRef());
    HPEN oldPenHandle =
      (HPEN)::SelectObject(deviceContextHandle,penHandle);

    ::MoveToEx(deviceContextHandle, startPoint.X(),
               startPoint.Y(), nullptr);
    ::LineTo(deviceContextHandle, endPoint.X(), endPoint.Y());

    ::SelectObject(deviceContextHandle, oldPenHandle);
    ::DeleteObject(penHandle);
  }

  void Graphics::DrawRectangle(Rect rect, Color penColor,
                               PenStyle penStyle /* = Solid */) {
    HPEN penHandle =
      ::CreatePen(penStyle, 0, penColor.ColorRef());

    LOGBRUSH lbBrush;
    lbBrush.lbStyle = BS_HOLLOW;
    HBRUSH brushHandle = ::CreateBrushIndirect(&lbBrush);

    HPEN oldPenHandle =
      (HPEN) ::SelectObject(deviceContextHandle,penHandle);
    HBRUSH oldBrushHandle =
      (HBRUSH)  ::SelectObject(deviceContextHandle, brushHandle);

    ::Rectangle(deviceContextHandle, rect.Left(), rect.Top(),
                rect.Right(), rect.Bottom());

    ::SelectObject(deviceContextHandle, oldBrushHandle);
    ::DeleteObject(brushHandle);

    ::SelectObject(deviceContextHandle, oldPenHandle);
    ::DeleteObject(penHandle);
  }

  void Graphics::FillRectangle(Rect rect, Color penColor,
               Color brushColor, PenStyle penStyle /* = Solid */){
    HPEN penHandle =
      ::CreatePen(penStyle, 0, penColor.ColorRef());
    HBRUSH brushHandle =
      ::CreateSolidBrush(brushColor.ColorRef());

    HPEN oldPenHandle =
      (HPEN)::SelectObject(deviceContextHandle,penHandle);
    HBRUSH oldBrushHandle =
      (HBRUSH) ::SelectObject(deviceContextHandle, brushHandle);

    ::Rectangle(deviceContextHandle, rect.Left(), rect.Top(),
                rect.Right(), rect.Bottom());

    ::SelectObject(deviceContextHandle, oldBrushHandle);
    ::DeleteObject(brushHandle);

    ::SelectObject(deviceContextHandle, oldPenHandle);
    ::DeleteObject(penHandle);
  }

  void Graphics::DrawEllipse(Rect rect, Color penColor,
                             PenStyle penStyle /* = Solid */) {
    HPEN penHandle =
      ::CreatePen(penStyle, 0, penColor.ColorRef());

    LOGBRUSH lbBrush;
    lbBrush.lbStyle = BS_HOLLOW;
    HBRUSH brushHandle = ::CreateBrushIndirect(&lbBrush);

    HPEN oldPenHandle =
      (HPEN)::SelectObject(deviceContextHandle,penHandle);
    HBRUSH oldBrushHandle =
      (HBRUSH) ::SelectObject(deviceContextHandle, brushHandle);

    ::Ellipse(deviceContextHandle, rect.Left(), rect.Top(),
              rect.Right(), rect.Bottom());

    ::SelectObject(deviceContextHandle, oldBrushHandle);
    ::DeleteObject(brushHandle);

    ::SelectObject(deviceContextHandle, oldPenHandle);
    ::DeleteObject(penHandle);
  }

  void Graphics::FillEllipse(Rect rect, Color penColor,
               Color brushColor, PenStyle penStyle /* = Solid */){
    HPEN penHandle =
      ::CreatePen(penStyle, 0, penColor.ColorRef());
    HBRUSH brushHandle =
      ::CreateSolidBrush(brushColor.ColorRef());

    HPEN oldPenHandle =
      (HPEN) ::SelectObject(deviceContextHandle,penHandle);
    HBRUSH oldBrushHandle =
      (HBRUSH) ::SelectObject(deviceContextHandle, brushHandle);

    ::Ellipse(deviceContextHandle, rect.Left(), rect.Top(),
              rect.Right(), rect.Bottom());

    ::SelectObject(deviceContextHandle, oldBrushHandle);
    ::DeleteObject(brushHandle);

    ::SelectObject(deviceContextHandle, oldPenHandle);
    ::DeleteObject(penHandle);
  }

  void Graphics::DrawText(Rect areaRect, String text, Font font,
                          Color textColor, Color backColor,
                          bool pointsToMeters /* = true */) {
    if (pointsToMeters) {
      font.PointsToLogical();
    }

    HFONT fontHandle = ::CreateFontIndirect(&font.LogFont());
    HFONT oldFontHandle =
      (HFONT) ::SelectObject(deviceContextHandle, fontHandle);

    ::SetTextColor(deviceContextHandle, textColor.ColorRef());
    ::SetBkColor(deviceContextHandle, backColor.ColorRef());

    RECT rectStruct = (RECT) areaRect;
    ::DrawText(deviceContextHandle, text.c_str(), text.length(),
               &rectStruct, DT_SINGLELINE |DT_CENTER |DT_VCENTER);

    ::SelectObject(deviceContextHandle, oldFontHandle);
    ::DeleteObject(fontHandle);
  }
};