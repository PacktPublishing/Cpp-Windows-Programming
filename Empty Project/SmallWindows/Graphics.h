namespace SmallWindows {
  class Window;
  enum PenStyle {Solid = PS_SOLID, Dash = PS_DASH, Dot = PS_DOT,
                 DashDot = PS_DASHDOT, DashDotDot =PS_DASHDOTDOT};

  class Graphics {
    private:
      Graphics(Window* windowPtr, HDC deviceContextHandle);

    public:
      int Save();
      void Restore(int saveId);

      void SetOrigin(Point centerPoint);
      void IntersectClip(Rect clipRect);

      void DrawLine(Point startPoint, Point endPoint,
                    Color penColor, PenStyle penStyle = Solid);
      void DrawRectangle(Rect rect, Color penColor,
                         PenStyle = Solid);
      void FillRectangle(Rect rect, Color penColor,
                       Color brushColor, PenStyle penStyle=Solid);
      void DrawEllipse(Rect rect, Color penColor,
                       PenStyle = Solid);
      void FillEllipse(Rect rect, Color penColor,
                       Color brushColor, PenStyle penStyle=Solid);
      void DrawText(Rect areaRect, String text, Font font,
                    Color textColor, Color backColor,
                    bool pointsToMeters = true);

      HDC GetDeviceContextHandle() const
                                   {return deviceContextHandle;}

    private:
      Window* windowPtr;
      HDC deviceContextHandle;

      friend LRESULT CALLBACK
        WindowProc(HWND windowHandle, UINT message,
                   WPARAM wordParam, LPARAM longParam);
      friend Graphics* StandardDialog::PrintDialog
                               (Window*parentPtr,int totalPages,
                                int& firstPage, int& lastPage,
                                int& copies, bool& sorted);
  };
};