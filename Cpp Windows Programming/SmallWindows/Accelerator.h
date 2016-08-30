namespace SmallWindows {
  enum Keys {KeyF1 = VK_F1, KeyF2 = VK_F2, KeyF3 = VK_F3,
             KeyF4 = VK_F4, KeyF5 = VK_F5, KeyF6 = VK_F6,
             KeyF7 = VK_F7, KeyF8 = VK_F8, KeyF9 = VK_F9,
             KeyF10 = VK_F10, KeyF11 = VK_F11, KeyF12 = VK_F12,
             KeyInsert = VK_INSERT, KeyDelete = VK_DELETE,
             KeyBackspace = VK_BACK, KeyTabulator = VK_TAB,
             KeyHome = VK_HOME, KeyEnd = VK_END,
             KeyPageUp = VK_PRIOR, KeyPageDown = VK_NEXT,
             KeyLeft = VK_LEFT, KeyRight = VK_RIGHT,
             KeyUp = VK_UP, KeyDown = VK_DOWN,
             KeySpace = VK_SPACE, KeyEscape = VK_ESCAPE,
             KeyReturn = VK_RETURN};

  class Accelerator {
    public:
      static void TextToAccelerator(String& text, int idemId,
                                    list<ACCEL>& acceleratorSet);
  };
};
