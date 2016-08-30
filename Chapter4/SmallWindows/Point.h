namespace SmallWindows {
  class Point {
    public:
      Point();
      Point(int x, int y);
      Point(const Point& point);
      Point& operator=(const Point& point);

      Point(const POINT& point);
      Point& operator=(const POINT& point);
      operator POINT() const;

      bool operator==(const Point& point) const;
      bool operator!=(const Point& point) const;
      bool operator<(const Point& point) const;
      bool operator<=(const Point& point) const;
      bool operator>(const Point& point) const;
      bool operator>=(const Point& point) const;

      friend Point Min(const Point& left, const Point& right);
      friend Point Max(const Point& left, const Point& right);

      Point& operator*=(double factor);
      friend Point operator*(const Point& point, double factor);
      friend Point operator*(double factor, const Point& point);

      Point& operator*=(pair<double,double> factorPair);
      friend Point operator*(const Point& point,
                             pair<double,double> factorPair);
      friend Point operator*(pair<double,double> factorPair,
                             const Point& point);

      Point& operator+=(const int distance);
      Point& operator-=(const int distance);
      friend Point operator+(const Point& left, int distance);
      friend Point operator-(const Point& left, int distance);

      Point& operator+=(const Size& size);
      Point& operator-=(const Size& size);
      friend Point operator+(const Point& point,const Size& size);
      friend Point operator-(const Point& point,const Size& size);

      Point& operator+=(const Point& point);
      Point& operator-=(const Point& point);
      friend Point operator+(const Point&left, const Point&right);
      friend Size operator-(const Point& left, const Point&right);

      bool WritePointToStream(ostream& outStream) const;
      bool ReadPointFromStream(istream& inStream);
      void WritePointToClipboard(InfoList& infoList) const;
      void ReadPointFromClipboard(InfoList& infoList);
      void WritePointToRegistry(String key) const; 
      void ReadPointFromRegistry(String key,
                            Point defaultPoint /* = ZeroPoint */);

      int X() const {return x;}
      int Y() const {return y;}
      int& X() {return x;}
      int& Y() {return y;}

    private:
      int x, y;
  };

  extern const Point ZeroPoint;
};