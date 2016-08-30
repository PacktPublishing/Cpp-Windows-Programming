#include "SmallWindows.h"

namespace SmallWindows {
  bool Clipboard::Open(const Window* windowPtr) {
    return (::OpenClipboard(windowPtr->WindowHandle()) != 0);
  }

  bool Clipboard::Close() {
    return (::CloseClipboard() != 0);
  }

  bool Clipboard::Clear() {
    return (::EmptyClipboard() != 0);
  }

  bool Clipboard::Available(unsigned int format) {
    return (::IsClipboardFormatAvailable(format) != 0);
  }

  bool Clipboard::WriteGeneric(unsigned int format,
                               InfoList& infoList) {
    int bufferSize = infoList.Size();
    HGLOBAL globalHandle = GlobalAlloc(GMEM_MOVEABLE, bufferSize);

    if (globalHandle != nullptr) {
      void* buffer = ::GlobalLock(globalHandle);

      if (buffer != nullptr) {
        infoList.ToBuffer(buffer);
        ::GlobalUnlock(globalHandle);
        ::SetClipboardData(format, globalHandle);
        return true;
      }
    }

    return false;
  }

  bool Clipboard::ReadGeneric(unsigned int format,
                              InfoList& infoList) {
    HGLOBAL globalHandle = ::GetClipboardData(format);

    if (globalHandle != nullptr) {
      void *buffer = ::GlobalLock(globalHandle);
      int bufferSize = ::GlobalSize(globalHandle);
      infoList.FromBuffer(buffer, bufferSize);
      ::GlobalUnlock(globalHandle);
      return true;
    }

    return false;
  }
};