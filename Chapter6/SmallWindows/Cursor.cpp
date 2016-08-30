#include "SmallWindows.h"

namespace SmallWindows {
  const CursorType Cursor::Normal = IDC_ARROW;
  const CursorType Cursor::Arrow = IDC_ARROW;
  const CursorType Cursor::ArrowHourGlass = IDC_APPSTARTING;
  const CursorType Cursor::Crosshair = IDC_CROSS;
  const CursorType Cursor::Hand = IDC_HAND;
  const CursorType Cursor::ArrowQuestionMark = IDC_HELP;
  const CursorType Cursor::IBeam = IDC_IBEAM;
  const CursorType Cursor::SlashedCircle = IDC_NO;
  const CursorType Cursor::SizeAll = IDC_SIZEALL;
  const CursorType Cursor::SizeNorthEastSouthWest = IDC_SIZENESW;
  const CursorType Cursor::SizeNorthSouth = IDC_SIZENS;
  const CursorType Cursor::SizeNorthWestSouthEast = IDC_SIZENWSE;
  const CursorType Cursor::SizeWestEast = IDC_SIZEWE;
  const CursorType Cursor::VerticalArrow = IDC_UPARROW;
  const CursorType Cursor::HourGlass = IDC_WAIT;

  void Cursor::Set(CursorType cursor) {
    ::SetCursor(::LoadCursor(nullptr, cursor));
  }
};