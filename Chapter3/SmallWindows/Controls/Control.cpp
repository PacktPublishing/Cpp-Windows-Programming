#include "..\\SmallWindows.h"

namespace SmallWindows {
  Control::Control(Dialog* parentPtr, Point topLeft,
                   Size controlSize, String className,
                   String text, int style)
   :Window(parentPtr),
    topLeft(topLeft),
    controlSize(controlSize),
    className(className),
    text(text),
    style(style) {
    controlId = parentPtr->AddControl(this);
  }

  void Control::AddControlInfo(InfoList& infoList) const {
    infoList.Align<DWORD>();
    infoList.AddValue<DWORD>(0);
    infoList.AddValue<DWORD>(0);
    infoList.AddValue<DWORD>(WS_CHILD | WS_VISIBLE | style);
    infoList.AddValue<WORD>(topLeft.X());
    infoList.AddValue<WORD>(topLeft.Y());
    infoList.AddValue<WORD>(controlSize.Width());
    infoList.AddValue<WORD>(controlSize.Height());
    infoList.AddValue<DWORD>(controlId);
    infoList.AddString<TCHAR>(className);
    infoList.AddString<TCHAR>(text);
    infoList.AddValue<WORD>(0);
  }
};