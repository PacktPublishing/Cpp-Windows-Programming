#ifndef _CURSOR_
#define _CURSOR_

namespace SmallWindows {
  class Window;

  class Cursor {
  public:
    Cursor(Window* pWindow);
    ~Cursor();

    void Show();
    void Hide();

    void SetSize(int xPos, int yPos);
    void SetPosition(int xPos, int yPos);

  private:
    Window* m_pWindow;
  };
};
#endif