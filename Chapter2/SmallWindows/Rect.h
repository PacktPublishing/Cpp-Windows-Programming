namespace SmallWindows {
  class Rect;
  extern const Rect ZeroRect;

  class Rect {
    public:
      Rect();
      Rect(int left, int top, int right, int bottom);
      Rect(const Rect& rect);
      Rect& operator=(const Rect& rect);
      Rect(Point topLeft, Point bottomRight);
      Rect(Point topLeft, Size size);

      Rect(const RECT& rect);
      Rect& operator=(const RECT& rect);
      operator RECT() const;

      bool operator==(const Rect& rect) const;
      bool operator!=(const Rect& rect) const;
      bool operator<(const Rect& rect) const;
      bool operator<=(const Rect& rect) const;
      bool operator>(const Rect& rect) const;
      bool operator>=(const Rect& rect) const;

      Rect& operator*=(double factor);
      friend Rect operator*(const Rect& rect, double factor);
      friend Rect operator*(double factor, const Rect& rect);

      Rect& operator*=(pair<double,double> factorPair);
      friend Rect operator*(const Rect& rect,
                            pair<double,double> factorPair);
      friend Rect operator*(pair<double,double> factorPair,
                            const Rect& rect);

      Rect& operator-=(const Size& size);
      Rect& operator+=(const Size& size);
      friend Rect operator-(const Rect& rect, const Size& size);
      friend Rect operator+(const Rect& rect, const Size& size);

      Rect& operator+=(const Point& point);
      Rect& operator-=(const Point& point);
      friend Rect operator+(const Rect& rect, const Point& point);
      friend Rect operator+(const Point& point, const Rect& rect);
      friend Rect operator-(const Rect& rect, const Point& point);

      int Width() const {return abs(right - left);}
      int Height() const {return abs(bottom - top);}
      Size GetSize() const {return Size(Width(), Height());}
      operator Size() const {return GetSize();}
      operator Point() const {return TopLeft();}

      Point TopLeft() const {return Point(left, top);}
      Point TopRight() const {return Point(right, top);}
      Point BottomLeft() const {return Point(left, bottom);}
      Point BottomRight() const {return Point(right, bottom);}

      void SetTopLeft(Point topLeft) {left = topLeft.X();
                                      right = topLeft.Y();}
      void SetBottomRight(Point bottomRight)
                         {right = bottomRight.X();
                          bottom = bottomRight.Y();}

      void Clear();
      void Normalize();
      bool PointInside(Point point) const;

      bool WriteRectToStream(ostream& outStream) const;
      bool ReadRectFromStream(istream& inStream);
      void WriteRectToClipboard(InfoList& infoList) const;
      void ReadRectFromClipboard(InfoList& infoList);
      void WriteRectToRegistry(String key) const;
      void ReadRectFromRegistry(String key,
                                Rect defaultRect = ZeroRect);

      int Left() const {return left;}
      int Right() const {return right;}
      int Top() const {return top;}
      int Bottom() const {return bottom;}

      int& Left() {return left;}
      int& Right() {return right;}
      int& Top() {return top;}
      int& Bottom() {return bottom;}

    private:
      int left, top, right, bottom;
  };
};
