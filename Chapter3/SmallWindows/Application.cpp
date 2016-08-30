#include "SmallWindows.h"

namespace SmallWindows {
  HINSTANCE Application::instanceHandle;
  String Application::applicationName;
  Window* Application::mainWindowPtr;

  void Application::RegisterWindowClasses(HINSTANCE
                                          instanceHandle) {
    Application::instanceHandle = instanceHandle;
    assert(instanceHandle != nullptr);

    WNDCLASS windowClass;
    memset(&windowClass, 0, sizeof windowClass);
    windowClass.hInstance = instanceHandle;
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground =
      (HBRUSH) GetStockObject(WHITE_BRUSH);

    windowClass.lpfnWndProc = WindowProc;
    windowClass.lpszClassName = TEXT("window");
    ::RegisterClass(&windowClass);

    windowClass.lpfnWndProc = DocumentProc;
    windowClass.lpszClassName = TEXT("document");
    ::RegisterClass(&windowClass);

    windowClass.lpfnWndProc = DocumentProc;
    windowClass.lpszClassName = TEXT("standarddocument");
    ::RegisterClass(&windowClass);
  }
  
  int Application::RunMessageLoop() {
    assert(!applicationName.empty());
    assert(mainWindowPtr != nullptr);

    MSG message;
    if (dynamic_cast<Document*>(mainWindowPtr) == nullptr) {
      while (::GetMessage(&message, nullptr, 0, 0)) {
        ::TranslateMessage(&message);
        ::DispatchMessage(&message);
      }
    }
    else {
      Document* documentPtr = (Document*) mainWindowPtr;
      int size = documentPtr->AcceleratorSet().size(), index = 0;
      ACCEL* acceleratorTablePtr = new ACCEL[size];
      assert(acceleratorTablePtr != nullptr);

      for (ACCEL accelerator : documentPtr->AcceleratorSet()) {
        acceleratorTablePtr[index++] = accelerator;
      }

      HACCEL acceleratorTable =
              ::CreateAcceleratorTable(acceleratorTablePtr, size);

      while (::GetMessage(&message, nullptr, 0, 0)) {
        if (!::TranslateAccelerator(mainWindowPtr->WindowHandle(),
                                    acceleratorTable, &message)) {
          ::TranslateMessage(&message);
          ::DispatchMessage(&message);
        }
      }

      delete [] acceleratorTablePtr;
    }

    return ((int) message.wParam);
  }
};